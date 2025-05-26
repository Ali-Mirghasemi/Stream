cmake_minimum_required(VERSION 3.16)

set(LIB_NAME "Stream")
string(TOUPPER "${LIB_NAME}" LIB_NAME_UPPER)

project(${LIB_NAME} VERSION 0.9.4 LANGUAGES C)

# === Global Options (can be set via environment variables) ===
option(${LIB_NAME_UPPER}_BUILD_STATIC_LIB "Build static library" OFF)
option(${LIB_NAME_UPPER}_BUILD_SHARED_LIB "Build shared library" OFF)
option(${LIB_NAME_UPPER}_BUILD_EXAMPLES "Build examples" OFF)

option(TARGET_ARCH "Target architecture" "none")

# === Custom Cross Compiler Support ===
option(USE_CUSTOM_COMPILER "Enable custom cross-compiler" OFF)
if (USE_CUSTOM_COMPILER)
    set(CMAKE_C_COMPILER "<path-to-cross-compiler>")
    set(CMAKE_SYSTEM_NAME "<target-system-name>")
    message(STATUS "Using custom cross-compiler: ${CMAKE_C_COMPILER}")
endif()

# === Target Platform Detection ===
if (TARGET_ARCH STREQUAL "arm")
    set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
    set(ARM ON)
    message(STATUS "Target: ARM")
elseif (TARGET_ARCH STREQUAL "riscv")
    set(CMAKE_C_COMPILER "riscv-none-embed-gcc")
    set(RISCV ON)
    message(STATUS "Target: RISC-V")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(WINDOWS ON)
    message(STATUS "Target: Windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX ON)
    message(STATUS "Target: Linux")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Generic")
    set(BARE_METAL ON)
    message(STATUS "Target: Bare-metal")
else()
    message(STATUS "Target: Unknown platform")
endif()

# === Directories ===
set(LIBRARY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src)
set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Examples)
set(EXAMPLES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Examples)

file(GLOB_RECURSE LIBRARY_SOURCES ${LIBRARY_SRC_DIR}/*.c)
file(GLOB_RECURSE LIBRARY_HEADERS ${LIBRARY_SRC_DIR}/*.h)

# === Decide Library Naming Based on Combination ===
# Convert boolean ON/OFF to 1/0 for math expressions
if(${LIB_NAME_UPPER}_BUILD_STATIC_LIB)
    set(BUILD_STATIC_LIB_NUM 1)
else()
    set(BUILD_STATIC_LIB_NUM 0)
endif()

if(${LIB_NAME_UPPER}_BUILD_SHARED_LIB)
    set(BUILD_SHARED_LIB_NUM 1)
else()
    set(BUILD_SHARED_LIB_NUM 0)
endif()

math(EXPR LIB_COUNT "${BUILD_STATIC_LIB_NUM} + ${BUILD_SHARED_LIB_NUM}")

# --- Shared Library ---
if (${LIB_NAME_UPPER}_BUILD_SHARED_LIB)
    if (LIB_COUNT EQUAL 1)
        add_library(${LIB_NAME} SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
        set(SHARED_TARGET ${LIB_NAME})
    else()
        add_library(${LIB_NAME}-Shared SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
        set(SHARED_TARGET ${LIB_NAME}-Shared)
    endif()
    add_library(${LIB_NAME}::Shared ALIAS ${SHARED_TARGET})
    target_include_directories(${SHARED_TARGET} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Src>
        $<INSTALL_INTERFACE:include>
    )
    target_compile_definitions(${SHARED_TARGET} PRIVATE ${LIB_NAME_UPPER}_EXPORTS)
    target_compile_features(${SHARED_TARGET} PUBLIC c_std_99)
    set_target_properties(${SHARED_TARGET} PROPERTIES
        PUBLIC_HEADER "${LIBRARY_HEADERS}"
    )
    install(TARGETS ${SHARED_TARGET}
        PUBLIC_HEADER DESTINATION include
    )
    # Enable automatic symbol export on Windows
    if(WIN32)
        set_target_properties(${SHARED_TARGET} PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS TRUE
            C_VISIBILITY_PRESET hidden
        )
    endif()
endif()

# --- Static Library ---
if (${LIB_NAME_UPPER}_BUILD_STATIC_LIB)
    if (LIB_COUNT EQUAL 1)
        add_library(${LIB_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
        set(STATIC_TARGET ${LIB_NAME})
    else()
        add_library(${LIB_NAME}-Static STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
        set(STATIC_TARGET ${LIB_NAME}-Static)
    endif()
    add_library(${LIB_NAME}::Static ALIAS ${STATIC_TARGET})
    target_include_directories(${STATIC_TARGET} PUBLIC ${LIBRARY_SRC_DIR})
    target_compile_features(${STATIC_TARGET} PUBLIC c_std_99)
    set_target_properties(${STATIC_TARGET} PROPERTIES
        PUBLIC_HEADER "${LIBRARY_HEADERS}"
    )
    install(TARGETS ${STATIC_TARGET}
        PUBLIC_HEADER DESTINATION include
    )
endif()

# === Examples ===
if (${LIB_NAME_UPPER}_BUILD_EXAMPLES)
    file(MAKE_DIRECTORY ${EXAMPLES_OUTPUT_DIR})

    set(EXAMPLE_NAMES
        ${LIB_NAME}-Example
        ${LIB_NAME}-Test
        ${LIB_NAME}-InOut
        ${LIB_NAME}-Limit
        ${LIB_NAME}-ReadLine
    )

    foreach(EXAMPLE_NAME ${EXAMPLE_NAMES})
        set(EXAMPLE_DIR ${EXAMPLES_DIR}/${EXAMPLE_NAME})
        file(GLOB EXAMPLE_SOURCES ${EXAMPLE_DIR}/*.c)

        if (EXAMPLE_SOURCES)
            add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
            target_include_directories(${EXAMPLE_NAME} PRIVATE ${LIBRARY_SRC_DIR})

            if (${LIB_NAME_UPPER}_BUILD_STATIC_LIB)
                target_link_libraries(${EXAMPLE_NAME} PRIVATE ${STATIC_TARGET})
            elseif (${LIB_NAME_UPPER}_BUILD_SHARED_LIB)
                target_link_libraries(${EXAMPLE_NAME} PRIVATE ${SHARED_TARGET})
            endif()

            # Copy shared library to examples output directory
            if (${LIB_NAME_UPPER}_BUILD_SHARED_LIB)
                add_custom_command(
                    TARGET ${EXAMPLE_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                        $<TARGET_FILE:${SHARED_TARGET}>
                        $<TARGET_FILE_DIR:${EXAMPLE_NAME}>
                    COMMENT "Copying shared library to examples directory"
                    VERBATIM
                )
            endif()

            set_target_properties(${EXAMPLE_NAME} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR})

            if (CMAKE_BUILD_TYPE STREQUAL "Release" AND CMAKE_STRIP)
                add_custom_command(TARGET ${EXAMPLE_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${EXAMPLE_NAME}>
                    COMMENT "Stripping symbols from ${EXAMPLE_NAME}")
            endif()
            message(STATUS "Added example: ${EXAMPLE_NAME}")
        else()
            message(WARNING "No source files found for example: ${EXAMPLE_NAME}")
        endif()
    endforeach()
endif()

install(DIRECTORY ${LIBRARY_SRC_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h")

# === Export Targets ===
install(
    TARGETS ${SHARED_TARGET} ${STATIC_TARGET}
    EXPORT ${LIB_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# === Generate ${LIB_NAME}Config.cmake.in Content ===
include(CMakePackageConfigHelpers)
set(CONFIG_IN_CONTENT [=[
@PACKAGE_INIT@

include("${CMAKE_CURRENT_LIST_DIR}/${LIB_NAME}Targets.cmake")

# Optional: Add version compatibility
check_required_components(${LIB_NAME})
]=])

# Write the content to a temporary .in file in the build directory
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake.in" "${CONFIG_IN_CONTENT}")

# === Generate Config Files ===
configure_package_config_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake.in  # Use generated file
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake/${LIB_NAME}
)

install(
    EXPORT ${LIB_NAME}Targets
    FILE ${LIB_NAME}Targets.cmake
    NAMESPACE ${LIB_NAME}::
    DESTINATION lib/cmake/${LIB_NAME}
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
    DESTINATION lib/cmake/${LIB_NAME}
)

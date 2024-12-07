# Minimum required version
cmake_minimum_required(VERSION 3.16)

# Project name and version
project(Stream VERSION 0.9.2 LANGUAGES C)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Define options for cross-platform support and project configuration
set(EXAMPLES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Examples)
option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(BUILD_STATIC_LIBS "Build static library" OFF)
option(BUILD_EXAMPLES "Build examples" ON)
option(TARGET_ARCH "Target architecture" "none")

# Option to specify a custom compiler
option(USE_CUSTOM_COMPILER "Enable custom cross-compiler" OFF)
if (USE_CUSTOM_COMPILER)
    set(CMAKE_C_COMPILER "<path-to-cross-compiler>")
    set(CMAKE_SYSTEM_NAME "<target-system-name>") 
    message(STATUS "Using custom cross-compiler: ${CMAKE_C_COMPILER}")
endif()

# Detect platform and adjust settings
if (TARGET_ARCH STREQUAL "arm")
    message(STATUS "Configuring for ARM architecture")
    # ARM-specific configurations
    set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
    set(ARM ON)
elseif (TARGET_ARCH STREQUAL "riscv")
    message(STATUS "Configuring for RISC-V architecture")
    # RISC-V-specific configurations
    set(CMAKE_C_COMPILER "riscv-none-embed-gcc")
    set(RISCV ON)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Configuring for Windows")
    # Windows-specific configurations
    set(WINDOWS ON)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
    # Linux-specific configurations
    set(LINUX ON)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Generic")
    message(STATUS "Configuring for bare-metal system")
    # Bare-metal-specific configurations
    set(BARE_METAL ON)
else()
    message(STATUS "Configuring for an unknown platform")
endif()

# Source directories
set(LIBRARY_SRC_DIR ${CMAKE_SOURCE_DIR}/Src)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/Examples)

# Collect library source files
file(GLOB_RECURSE LIBRARY_SOURCES ${LIBRARY_SRC_DIR}/*.c)
file(GLOB_RECURSE LIBRARY_HEADERS ${LIBRARY_SRC_DIR}/*.h)

# Add static library
if (BUILD_STATIC_LIBS)
    add_library(${PROJECT_NAME}-Static STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
    message(STATUS "Building static library")
endif()

# Add shared library
if (BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
    message(STATUS "Building shared library")
    if(WIN32)
        target_compile_definitions(${PROJECT_NAME} PRIVATE _WINDOWS)
    endif()
endif()

# Include directory for the library
if (BUILD_SHARED_LIBS)
    target_include_directories(${PROJECT_NAME} PUBLIC ${LIBRARY_SRC_DIR})
    target_compile_definitions(${PROJECT_NAME} PRIVATE STREAM_EXPORTS)
endif()
if (BUILD_STATIC_LIBS)
    target_include_directories(${PROJECT_NAME}-Static PUBLIC ${LIBRARY_SRC_DIR})
endif()

# Add examples if enabled
if (BUILD_EXAMPLES)
    # Specify the example names explicitly
    set(EXAMPLE_NAMES
        Stream-Example
        Stream-Test
        Stream-InOut
        Stream-Limit
        Stream-ReadLine
    )
    file(MAKE_DIRECTORY ${EXAMPLES_OUTPUT_DIR})

    foreach(EXAMPLE_NAME ${EXAMPLE_NAMES})
        set(EXAMPLE_DIR ${EXAMPLES_DIR}/${EXAMPLE_NAME})
        file(GLOB EXAMPLE_SOURCES ${EXAMPLE_DIR}/*.c)

        # Only add the example if source files are found
        if (EXAMPLE_SOURCES)
            add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
            target_include_directories(${EXAMPLE_NAME} PRIVATE ${LIBRARY_SRC_DIR})
            target_link_libraries(${EXAMPLE_NAME} PRIVATE $<IF:$<BOOL:${BUILD_STATIC_LIBS}>,${PROJECT_NAME}-Static,${PROJECT_NAME}>)
            # Force the example's output directory
            set_target_properties(${EXAMPLE_NAME} PROPERTIES 
                RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR})

             # Strip symbols for Release configuration
            if (CMAKE_BUILD_TYPE STREQUAL "Release")
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

# Installation rules
if (BUILD_SHARED_LIBS)
    install(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
    )
    install(DIRECTORY ${LIBRARY_SRC_DIR}/
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
    )
endif()
if (BUILD_STATIC_LIBS)
    install(TARGETS ${PROJECT_NAME}-Static
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
    )
    install(DIRECTORY ${LIBRARY_SRC_DIR}/
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
    )
endif()

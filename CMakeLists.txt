# Set library name
set(LIB_NAME STREAM)

# Find all source files
file(GLOB_RECURSE ${LIB_NAME}_SRCS
    "Src/*.h"
    "Src/*.c"
)

# Find library sources
file(GLOB_RECURSE LIB_SRC_FILES ./*.c)

# Declare the library
add_library(${LIB_NAME}_LIB STATIC
    ${${LIB_NAME}_SRCS}
)

# Find library headers
file(GLOB_RECURSE LIB_INC_FILES ./*.h)


# Set include directories
target_include_directories(${LIB_NAME}_LIB PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
)

# Install library
install(TARGETS ${LIB_NAME}_LIB DESTINATION lib)
install(FILES ${LIB_INC_FILES} DESTINATION include)

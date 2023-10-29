cmake_minimum_required(VERSION 3.25)
project(serialPort C)

set(CMAKE_C_STANDARD 11)
#set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES
        main.c
        uart_sock.c
        userCodes.c
        Str/Str.c
        Stream/StreamBuffer.c
        Stream/UARTStream.c
        Stream/buffer.c
        Stream/OutputStream.c
        Stream/InputStream.c
        arduino/arduino.c
        )

#target_include_directories(${PROJECT_NAME} PRIVATE ${ROOT_DIR} ${ROOT_DIR}/Stream ${ROOT_DIR}/Str)
include_directories(parser Str Stream arduino ec200)
add_executable(serialPort ${SOURCES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif ()
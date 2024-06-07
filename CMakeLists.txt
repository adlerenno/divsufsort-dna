cmake_minimum_required(VERSION 3.18)
project(dss C)

set(CMAKE_C_STANDARD 11)

add_definitions(-D_LARGEFILE_SOURCE -D_LARGE_FILES -D_FILE_OFFSET_BITS=64)

set(SOURCES
        main.c
        raw_input_dna.c
        raw_output_dna.c
)

set(INCLUDES
        ${CMAKE_CURRENT_BINARY_DIR}
        /usr/local/include
        /usr/local/lib
)

# add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_executable(${PROJECT_NAME} main.c)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

link_directories("/usr/local/lib")
target_link_libraries(${PROJECT_NAME} divsufsort)
# target_link_libraries(${PROJECT_NAME} PRIVATE /usr/local/lib/libdivsufsort64.dylib)
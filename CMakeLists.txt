cmake_minimum_required(VERSION 3.18)
project(bwt_constructions C)

set(CMAKE_C_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl")
else()
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
endif()

option(BUILD_DSS "Build the DSS executable" ON)
option(BUILD_LIBSAIS "Build the libsais executable" OFF)
option(BUILD_LIBCUBWT "Build the libcubwt executable" OFF)

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

if (BUILD_DSS)
    # add_library(${PROJECT_NAME} SHARED ${SOURCES})
    # target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
    add_executable(dss ${SOURCES})
    target_compile_definitions(dss PRIVATE -D_LARGEFILE_SOURCE -D_LARGE_FILES -D_FILE_OFFSET_BITS=64 -DDIVSUFSORT)
    target_include_directories(dss PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/../include"
            "${CMAKE_CURRENT_BINARY_DIR}/../include")
    target_link_directories(dss PRIVATE "/usr/local/lib")
    target_link_libraries(dss PRIVATE divsufsort64)
    # target_link_libraries(${PROJECT_NAME} PRIVATE /usr/local/lib/libdivsufsort64.dylib)
endif ()

if (BUILD_LIBSAIS)
    add_executable(libsais
            ${SOURCES}
            libsais/include/libsais.h
            libsais/include/libsais64.h
            libsais/src/libsais64.c
            libsais/src/libsais.c)
    if(OpenMP_C_FOUND)
        target_link_libraries(libsais PRIVATE OpenMP::OpenMP_C)
        target_compile_options(libsais PRIVATE ${OpenMP_C_FLAGS})
    endif()
    target_compile_options(libsais PRIVATE -Ofast -march=skylake -DNDEBUG)
    target_compile_definitions(libsais PRIVATE -D_LARGEFILE_SOURCE -D_LARGE_FILES -D_FILE_OFFSET_BITS=64 -DLIBSAIS)
    target_include_directories(libsais PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/../include"
            "${CMAKE_CURRENT_BINARY_DIR}/../include")
endif()

if (BUILD_LIBCUBWT)
    enable_language(CUDA)
    add_executable(libcubwt
            ${SOURCES}
            libcubwt/libcubwt.cuh
            libcubwt/libcubwt.cu)
    set_target_properties(libcubwt PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_compile_options(libcubwt PRIVATE -march=native --use_fast_math )
    target_compile_definitions(libcubwt PRIVATE -D_LARGEFILE_SOURCE -D_LARGE_FILES -D_FILE_OFFSET_BITS=64 -DLIBCUBWT)
    target_include_directories(libcubwt PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/../include"
            "${CMAKE_CURRENT_BINARY_DIR}/../include")
endif()

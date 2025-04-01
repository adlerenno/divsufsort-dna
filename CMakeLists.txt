cmake_minimum_required(VERSION 3.18)
project(bwt_constructions C)

set(CMAKE_C_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl")
else()
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
endif()

option(BUILD_DSS "Build the DSS executable" OFF)
option(BUILD_LIBSAIS "Build the libsais executable" ON)
option(LIBSAIS_USE_OPENMP "Use OpenMP for parallelization" ON)
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
    set(LIBSAIS_LIBRARY_TYPE STATIC)
    add_library(libsais ${LIBSAIS_LIBRARY_TYPE})
    target_sources(libsais PRIVATE
            include/libsais.h
            include/libsais16.h
            include/libsais16x64.h
            include/libsais64.h
            src/libsais.c
            src/libsais16.c
            src/libsais16x64.c
            src/libsais64.c
    )

    if(LIBSAIS_USE_OPENMP)
        find_package(OpenMP REQUIRED)
    endif()

    if(LIBSAIS_USE_OPENMP AND OpenMP_C_FOUND)
        target_compile_definitions(libsais PUBLIC LIBSAIS_OPENMP)
        target_link_libraries(libsais PRIVATE OpenMP::OpenMP_C)
        message("-- OpenMP found.")
    else()
        message("-- OpenMP not found.")
    endif()

    target_include_directories(libsais PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    add_executable(sais-cli ${SOURCES})
    target_compile_definitions(sais-cli PRIVATE -D_LARGEFILE_SOURCE -D_LARGE_FILES -D_FILE_OFFSET_BITS=64 -DLIBSAIS)
    target_include_directories(sais-cli PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/../include"
            "${CMAKE_CURRENT_BINARY_DIR}/../include")
    target_link_libraries(sais-cli PRIVATE libsais)
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

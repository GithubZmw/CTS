cmake_minimum_required(VERSION 3.28)
project(socket)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE COMMON_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/output/*.cc
)

set(MAIN_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/test/startAggregator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/startTA.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/startUAV.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/startVerifier.cpp
)

set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/libsh
        ${CMAKE_CURRENT_SOURCE_DIR}/libsh/miracl
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/proto
)

set(LINK_LIBS
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/libbrpc.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/bls12381.a
        gmp
        gmpxx
        protobuf::libprotobuf
        gflags
        leveldb
        pthread
        OpenSSL::SSL
        OpenSSL::Crypto
)

foreach(MAIN_SRC ${MAIN_SOURCES})
    get_filename_component(EXE_NAME ${MAIN_SRC} NAME_WE)
    add_executable(${EXE_NAME} ${COMMON_SRC} ${MAIN_SRC})
    target_include_directories(${EXE_NAME} PUBLIC ${INCLUDE_DIRS})
    target_link_libraries(${EXE_NAME} PRIVATE ${LINK_LIBS})
endforeach()

find_package(Protobuf REQUIRED)
find_package(gflags REQUIRED)
find_package(OpenSSL REQUIRED)
find_library(GMP gmp REQUIRED)
find_library(GMPXX gmpxx REQUIRED)

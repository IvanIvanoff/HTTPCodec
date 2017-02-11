cmake_minimum_required(VERSION 2.6)
set(TARGET HTTPCodecTest)
project(${TARGET})

find_package(Boost REQUIRED)

set (CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES
        http_codec_test.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

install(TARGETS ${TARGET} RUNTIME DESTINATION bin)

cmake_minimum_required(VERSION 3.10)
project(converter)  


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


include_directories(include)


file(GLOB SOURCES "src/*.cpp")


add_executable(converter ${SOURCES})  


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(converter PRIVATE -fsanitize=address -fsanitize=leak)
    target_link_libraries(converter PRIVATE -fsanitize=address -fsanitize=leak)
endif()


if (UNIX)
    target_link_libraries(converter PRIVATE m assimp)
endif()



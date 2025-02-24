cmake_minimum_required(VERSION 3.2)
set (CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RELEASE_OPTIONS "-Wall" "-O2")
set(DEBUG_OPTIONS "-Wall")

set(PROJECT_NAME "Tests")
set(SRC_LIST "src/main.cpp")
set(INCLUDE_DIR "include/")
set(TARGET "main")

project(${PROJECT_NAME})


add_executable( ${TARGET} ${SRC_LIST} )

target_compile_options(${TARGET} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
target_compile_options(${TARGET} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")

target_include_directories(${TARGET} PUBLIC ${INCLUDE_DIR})

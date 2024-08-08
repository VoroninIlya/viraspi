#Project version minimum
cmake_minimum_required(VERSION 3.16.4)
set(PROJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#Project name
project(HelloWorldProject VERSION 1.0.0)
#source files directory
set(SOURCES "src/main.c")

set(ALL_FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${ALL_FILES})
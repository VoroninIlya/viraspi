#Project version minimum
cmake_minimum_required(VERSION 3.22)

set(PROJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#Project name
project(HelloWorldProject VERSION 1.0.0)
#source files directory
set(SOURCES "${PROJ_PATH}/src/main.c")

add_subdirectory(thirdparty)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC 
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_INCLUDE_PATH}
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/pigpio
)

target_link_libraries(${PROJECT_NAME} PRIVATE pigpio pigpiod_if pigpiod_if2 -lpthread -lrt)
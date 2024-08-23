#Project version minimum
cmake_minimum_required(VERSION 3.22)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#Project name
project(HelloWorldProject VERSION 1.0.0)
#source files directory
set(SOURCES "${PROJ_PATH}/src/main.cpp"
    "${PROJ_PATH}/src/hcsr04callbacks/hcsr04callbacks.cpp"
    "${PROJ_PATH}/thirdparty/pigpio/EXAMPLES/CPP/ROTARY_ENCODER/rotary_encoder.cpp"
)

add_subdirectory(thirdparty)
add_subdirectory(${PROJ_PATH}/modules/unity)
add_subdirectory(${PROJ_PATH}/modules/gtest)
add_subdirectory(${PROJ_PATH}/modules/vihcsr04)
add_subdirectory(${PROJ_PATH}/modules/viwebserver)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_INCLUDE_PATH}
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/pigpio
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/mongoose
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/mongoose/src
    ${PROJ_PATH}/thirdparty/pigpio/EXAMPLES/CPP/ROTARY_ENCODER/
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    vihcsr04 vihcsr04cpp viwebserver mongoose
    pigpio pigpiod_if pigpiod_if2 -lpthread -lrt)
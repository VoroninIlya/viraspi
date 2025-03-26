#Project version minimum
cmake_minimum_required(VERSION 3.22)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#Project name
project(viraspi VERSION 1.0.0)
#source files directory
set(SOURCES "${PROJ_PATH}/src/main.cpp"
    "${PROJ_PATH}/src/time/time.cpp"
    "${PROJ_PATH}/src/hcsr04callbacks/hcsr04callbacks.cpp"
    "${PROJ_PATH}/src/webapihandler/webapihandler.cpp"
    "${PROJ_PATH}/src/webapihandler/connectionhandler.cpp"
    "${PROJ_PATH}/src/loghandler/loghandler.cpp"
    "${PROJ_PATH}/thirdparty/pigpio/EXAMPLES/CPP/ROTARY_ENCODER/rotary_encoder.cpp"
)

add_subdirectory(thirdparty)
add_subdirectory(${PROJ_PATH}/modules/unity)
add_subdirectory(${PROJ_PATH}/modules/gtest)
add_subdirectory(${PROJ_PATH}/modules/vihcsr04)
add_subdirectory(${PROJ_PATH}/modules/viwebapi)
add_subdirectory(${PROJ_PATH}/modules/viasyncworker)
add_subdirectory(${PROJ_PATH}/modules/vilogger)

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
    vihcsr04 vihcsr04cpp mongoose viwebapi viasyncworker
    vilogger
    pigpio pigpiod_if pigpiod_if2 -lpthread -lrt)

#
# Test project settings
#
project(tst_viraspi)

include(CTest)

set(TST_EXECUTABLE tst_viraspi)
set_property(GLOBAL PROPERTY sources_list)
set_property(GLOBAL PROPERTY include_paths_list)

#
# Symbols definition
#
#set(symbols_SYMB
    # Put here your symbols (preprocessor defines), one in each line
    # Encapsulate them with double quotes for safety purpose
#)

# Executable files
add_executable(${TST_EXECUTABLE} 
    ${PROJ_PATH}/tests/main/main.cpp 
    ${tst_sources})

message("${tst_includes}" ${tst_includes})

# Include paths
target_include_directories(${TST_EXECUTABLE} PRIVATE 
  ${tst_includes}
  ${UNITY_ROOT_PATH}/extras/fixture/src
  ${UNITY_ROOT_PATH}/extras/memory/src
  ${GTEST_ROOT_PATH}/googlemock/include
)

# Compilation definition information
target_compile_definitions(${TST_EXECUTABLE} PUBLIC
    #WIN32
    #_DEBUG
    #CONSOLE
)

# Compiler options
target_compile_options(${TST_EXECUTABLE} PUBLIC
    -g
    -fprofile-arcs 
    -ftest-coverage
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(${TST_EXECUTABLE} unity gmock -lgcov -fprofile-arcs -ftest-coverage -pthread)
add_test(NAME tst_viraspi COMMAND tst_viraspi)

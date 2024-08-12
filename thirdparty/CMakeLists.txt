
# libpigpio.(so|a)
add_library(pigpio 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/pigpio.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/command.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/custom.cext)

# libpigpiod_if.(so|a)
add_library(pigpiod_if 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/pigpiod_if.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/command.c)

# libpigpiod_if2.(so|a)
add_library(pigpiod_if2
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/pigpiod_if2.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/command.c)

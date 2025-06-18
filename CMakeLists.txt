# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(rpico2-motor-driver C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(rpico2-motor-driver main.cpp 
        rpico-gpio/gpio.cpp
        rpico-pwm/pwm.cpp
        rpico-motor/motor.cpp
        rpico-encoder-plus/qenc.cpp
        generic-pid/pid.cpp
        rpico-adc/adc.cpp
        rpico-servo/servo.cpp            
)

pico_set_program_name(rpico2-motor-driver "rpico2-motor-driver")
pico_set_program_version(rpico2-motor-driver "0.1")

# Generate PIO header
pico_generate_pio_header(rpico2-motor-driver ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(rpico2-motor-driver 0)
pico_enable_stdio_usb(rpico2-motor-driver 1)

# Add the standard library to the build
target_link_libraries(rpico2-motor-driver
        pico_stdlib
        )

# Add the standard include files to the build
target_include_directories(rpico2-motor-driver PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/rpico-gpio
        ${CMAKE_CURRENT_LIST_DIR}/rpico-pwm
        ${CMAKE_CURRENT_LIST_DIR}/rpico-motor
        ${CMAKE_CURRENT_LIST_DIR}/rpico-encoder-plus
        ${CMAKE_CURRENT_LIST_DIR}/generic-pid
        ${CMAKE_CURRENT_LIST_DIR}/rpico-adc
        ${CMAKE_CURRENT_LIST_DIR}/rpico-servo
)

# Add any user requested libraries
target_link_libraries(rpico2-motor-driver 
        hardware_pio
        hardware_pwm
        hardware_adc
        hardware_gpio
        )

pico_add_extra_outputs(rpico2-motor-driver)


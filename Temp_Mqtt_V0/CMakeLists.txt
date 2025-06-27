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
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Temp_Mqtt_V0 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Temp_Mqtt_V0 Temp_Mqtt_V0.cpp )

pico_set_program_name(Temp_Mqtt_V0 "Temp_Mqtt_V0")
pico_set_program_version(Temp_Mqtt_V0 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Temp_Mqtt_V0 0)
pico_enable_stdio_usb(Temp_Mqtt_V0 1)

# Add the standard library to the build
target_link_libraries(Temp_Mqtt_V0
        pico_stdlib
        hardware_adc
        )

# Add the standard include files to the build
target_include_directories(Temp_Mqtt_V0 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Temp_Mqtt_V0 
        pico_cyw43_arch_lwip_poll
        tinyusb_board
        pico_lwip_mqtt
        )

pico_add_extra_outputs(Temp_Mqtt_V0)


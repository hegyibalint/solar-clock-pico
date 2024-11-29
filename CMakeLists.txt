cmake_minimum_required(VERSION 3.30)
include(FetchContent)

# Settings
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable colors on Ninja
if(CMAKE_GENERATOR MATCHES "Ninja")
    add_compile_options(-fdiagnostics-color=always)
endif()

# Including and initializing the Raspberry Pico SDK
set(PICO_BOARD pico_w)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
pico_sdk_init()

# Base setup for the project
project(solar_clock)
add_executable(solar_clock src/main.c)

# Fetch the jsmn library
FetchContent_Declare(
    jsmn
    GIT_REPOSITORY https://github.com/zserge/jsmn
    GIT_TAG        master
)
FetchContent_MakeAvailable(jsmn)

target_include_directories(solar_clock PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${jsmn_SOURCE_DIR}
)
target_link_libraries(solar_clock 
    pico_stdlib 
    pico_stdio_rtt
    pico_lwip_http
    pico_cyw43_arch_lwip_poll
)
pico_add_extra_outputs(solar_clock)

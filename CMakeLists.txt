# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(hugecan C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(hugecan
    mcp2515/mcp2515.cpp

    hugecan.cpp
)

# Create a library for can2040
add_library(can2040
    can2040/can2040.c
)
add_library(mcp2515
    mcp2515/mcp2515.cpp)

# Link the Pico SDK libraries to can2040 (for hardware/<xxx> support)
target_link_libraries(can2040 pico_stdlib hardware_spi hardware_uart hardware_dma hardware_irq hardware_sync cmsis_core)
target_link_libraries(mcp2515
    pico_stdlib
    hardware_spi
)

# Link can2040 to the hugecan executable
target_link_libraries(hugecan
    pico_stdlib
    can2040
    mcp2515
)

# Add the standard include files to the build
target_include_directories(hugecan PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add include directories for can2040
target_include_directories(can2040 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}  # Add this directory to the include path for can2040.h
)
target_include_directories(mcp2515 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}  # Add this directory to the include path for can2040.h
)
# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(hugecan 0)
pico_enable_stdio_usb(hugecan 0)

pico_set_program_name(hugecan "hugecan")
pico_set_program_version(hugecan "0.1")

pico_add_extra_outputs(hugecan)
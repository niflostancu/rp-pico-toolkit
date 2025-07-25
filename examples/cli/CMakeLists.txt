cmake_minimum_required(VERSION 3.12)

get_filename_component(PARENT_DIR ../../ ABSOLUTE)

# Pull in SDK (must be before project)
include(${PARENT_DIR}/pico_sdk_import.cmake)

project(rp_cli_example C CXX ASM)
set(CMAKE_C_STANDARD 11)

if(PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_subdirectory(${PARENT_DIR} rp_toolkit)
include_directories(${PARENT_DIR})

if(TARGET tinyusb_device)
  add_executable(rp_cli_example main.c)

  # pull in common dependencies
  target_link_libraries(rp_cli_example PUBLIC pico_stdlib rp_cli rp_cli_rp_stdio)
  if(PICO_CYW43_SUPPORTED)
    target_link_libraries(rp_cli_example pico_cyw43_arch_none)
  endif()
  # workaround for USB Errata RP2040-E5
  target_compile_definitions(rp_cli_example PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

  pico_enable_stdio_usb(rp_cli_example 1)
  pico_enable_stdio_uart(rp_cli_example 0)

  # create map/bin/hex/uf2 file etc.
  pico_add_extra_outputs(rp_cli_example)

elseif(PICO_ON_DEVICE)
  message(WARNING "TinyUSB submodule is not initialized in the SDK")
endif()

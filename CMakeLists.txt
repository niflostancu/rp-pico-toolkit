# Declare the base library

add_library(lib_rp_toolkit INTERFACE)
target_include_directories(lib_rp_toolkit INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")

# Include all modules' cmake scripts
add_subdirectory(cli)

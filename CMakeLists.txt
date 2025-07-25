# Core interface library with basic includes & utils
add_library(rp_toolkit_core INTERFACE)
target_include_directories(rp_toolkit_core INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")

if((NOT DEFINED RP_LIB_CONFIG_HEADER) AND (TARGET pico_base_headers))
	set(RP_LIB_CONFIG_HEADER "pico/config.h")
	target_link_libraries(rp_toolkit_core INTERFACE pico_base_headers)
endif()
target_compile_definitions(rp_toolkit_core INTERFACE RP_LIB_CONFIG_HEADER="${RP_LIB_CONFIG_HEADER}")

# Include all modules' cmake scripts
add_subdirectory(rp_cli)

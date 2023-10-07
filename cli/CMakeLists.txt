# Stdio-based CLI library for embedded programs

if (RP_TOOLKIT_CLI_ENABLED)
	target_sources(lib_rp_toolkit PRIVATE cli.c)
	target_link_libraries(lib_rp_toolkit PUBLIC pico_stdlib)
endif()

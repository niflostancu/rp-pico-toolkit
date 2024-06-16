# Stdio-based CLI library for embedded programs
add_library(lib_rp_cli)

target_sources(lib_rp_cli PRIVATE ./cli.c)
target_link_libraries(lib_rp_cli PUBLIC lib_rp_toolkit PRIVATE pico_stdlib)

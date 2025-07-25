# Generic CLI/shell library
add_library(rp_cli)
target_sources(rp_cli PRIVATE cli.c cmd_parser.c readline.c)
target_link_libraries(rp_cli PUBLIC rp_toolkit_core)

# Stdio-based CLI interface
add_library(rp_cli_rp_stdio)
target_sources(rp_cli_rp_stdio PRIVATE rp_stdio.c)
target_link_libraries(rp_cli_rp_stdio PRIVATE rp_cli pico_stdlib)

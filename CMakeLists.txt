# Include for libraries

# A common library which will contain all sub-modules
add_library(lib_rp_toolkit)
target_include_directories(lib_rp_toolkit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include all modules' cmake scripts
add_subdirectory(cli)
add_subdirectory(main)


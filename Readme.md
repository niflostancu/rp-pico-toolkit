# RaspberryPI Pico Toolkit

This is a library implementing common functionality required for building
interactive RaspberryPI Pico (RP2* family)-based applications.

**Features:**

- CLI framework with basic ANSI escape codes (line editing & history);

_Future/WIP_:

- Debugging routines;
- Configuration storage with USB MSC (ini file) & CLI frontends;
- wifi-based utilities (e.g., UART over TCP);

## Usage

We recommend to include this library as a git submodule:

```bash
git submodule add https://github.com/niflostancu/rp-pico-toolkit.git lib/rp_toolkit
```

Then, set your desired module options & include the library's directory into
your `CMakeLists.txt`, e.g.:

```cmake
add_subdirectory(lib/rp_toolkit)
# ...
target_link_libraries(your_app PRIVATE rp_cli rp_cli_rp_stdio)
```


# RaspberryPI Pico Toolkit

This is a library implementing common functionality required for building
interactive RaspberryPI Pico (RP2040)-based applications.

**Features:**

- CLI framework with basic ANSI escape codes (line editing & history);
- WIP: Configuration storage with USB MSC (ini file) & CLI frontends;
- TODO: wifi-based utilities (e.g., UART over TCP);

## Usage

We recommend to include this library as a git submodule:

```bash
git submodule add https://github.com/niflostancu/rp-pico-toolkit.git rp-toolkit
```

Then, set your desired module options & include the library's directory into
your `CMakeLists.txt`:

```cmake
set(RP_TOOLKIT_CLI_ENABLED true)
add_subdirectory(rp-toolkit)
```


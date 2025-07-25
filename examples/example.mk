# Makefile for easy RPI Pico example build / flash

NAME ?= example
BUILD_DIR ?= build
PICO_W ?=
V ?=

CMAKE_ARGS = -DCMAKE_EXPORT_COMPILE_COMMANDS=YES $(if $(PICO_W),-DPICO_BOARD=pico_w,)

# set to 1 to force reboot before flashing
FORCE = 

build:
	@mkdir -p "$(BUILD_DIR)" && \
	ln -sf build/compile_commands.json .; \
	( cd "$(BUILD_DIR)"; cmake .. $(CMAKE_ARGS) && make VERBOSE=$(V); )

flash:
	picotool load $(if $(FORCE),-f) "$(BUILD_DIR)/$(NAME).elf"

clean:
	rm -rf "$(BUILD_DIR)"

.PHONY: build flash clean

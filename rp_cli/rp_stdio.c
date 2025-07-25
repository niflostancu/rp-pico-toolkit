#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "pico/stdio.h"

#include "./rp_stdio.h"


int _write(int handle, const char *buffer, int length);

static int _rp_stdio_write(const char* str, size_t len);
static void _rp_stdio_flush();
static int _rp_stdio_read();

/** RP stdio functions map */
const struct rp_cli_io_funcs rp_cli_stdio_funcs = {
    .write = _rp_stdio_write,
    .flush_output = _rp_stdio_flush,
    .read_char = _rp_stdio_read,
};

/**
 * Prefer the raw `_write` instead of using `puts()` since it accepts a length
 * argument.
 */
static int _rp_stdio_write(const char* str, size_t len)
{
    return _write(1, str, len);
}

/**
 * Reads one character from the Pico SDK standard I/O provider.
 */
static int _rp_stdio_read()
{
    int raw = getchar_timeout_us(RP_CLI_STDIO_READ_TIMEOUT_US);
    if (raw == PICO_ERROR_TIMEOUT)
        return -1;
    return raw;
}

static void _rp_stdio_flush()
{
    stdio_flush();
}

#ifndef _LIB_RP_CLI_PICO_STDIO_H
#define _LIB_RP_CLI_PICO_STDIO_H
/**
 * RP Pico stdio-based IO implementation for the CLI.
 */

#include "rp_extra_config.h"
#include "./io.h"

#ifndef RP_CLI_STDIO_READ_TIMEOUT_US
/** Pico read timeout, in microseconds. */
#define RP_CLI_STDIO_READ_TIMEOUT_US  1
#endif


/** CLI I/O callbacks using the Pico SDK `stdio` implementation. */
extern const struct rp_cli_io_funcs rp_cli_stdio_funcs;

#endif

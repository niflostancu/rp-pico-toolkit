#ifndef _LIB_RP_CLI_IO_DEFS_H
#define _LIB_RP_CLI_IO_DEFS_H
/**
 * Common CLI abstractions & definitions.
 *
 * All routines should return 0 or number of bytes read / written on
 * success, negative on error.
 */

#include <stddef.h>


/** CLI write callback function */
typedef int (*rp_cli_write_func_t)(const char* str, size_t len);

/** CLI read single char (user input) callback function */
typedef int (*rp_cli_read_char_func_t)();

/** CLI output flush callback function */
typedef void (*rp_cli_flush_func_t)();


/**
 * I/O driver with functions required by CLI routines.
 *
 * Note: methods should not block, but be reliably buffered (otherwise, CLI
 * output fragments may be lost).
 */
struct rp_cli_io_funcs {
    /**
     * Writes bytes to the output.
     * Should be reliable (must return an error if all bytes could not be
     * written so the backend may cache and try later -- if implemented).
     */
    rp_cli_write_func_t write;

    /** Requests the output buffer flushed as soon as possible. */
    rp_cli_flush_func_t flush_output;

    /** Read a character (if available) and return it, -1 otherwise. */
    rp_cli_read_char_func_t read_char;
};

#endif

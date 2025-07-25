#ifndef _LIB_RP_CLI_CMD_PARSER_H
#define _LIB_RP_CLI_CMD_PARSER_H
/**
 * Simple CLI command line parser (string tokenizer).
 */

#include <stdint.h>

#include "rp_util/preprocessor.h"


#ifdef RP_LIB_CONFIG_HEADER
#include RP_LIB_CONFIG_HEADER
#endif

#ifndef RP_CLI_MAX_CMD_ARGS
/** Maximum number of allowed command arguments. */
#define RP_CLI_MAX_CMD_ARGS  16
#endif

/**
 * Structure to store the tokenized arguments of a command.
 *
 * Must be properly initialized with a buffer (+ max length) using the 
 * `rp_cli_cmd_token_list_init` or `RP_CLI_CMD_TOKEN_LIST_SINIT`.
 */
struct rp_cli_cmd_tokens {
    /** Internal buffer to store the obtained tokens. */
    char *_buf;
    /** Max. buffer length */
    uint_fast16_t _buf_size;
    /** Current tokenized arguments count. */
    int argc;
    /** Array with the individual tokens. */
    const char *argv[RP_CLI_MAX_CMD_ARGS];
};

/** Statically initializes a CLI tokens list. */
#define RP_CLI_CMD_TOKEN_LIST_SINIT(_varname, _buflen) \
    static char CAT(_varname, __buf)[_buflen]; \
    static struct rp_cli_cmd_tokens _varname = { \
        ._buf = CAT(_varname, __buf), \
        ._buf_size = _buflen, \
        .argc = 0, .argv = {0}, \
    };


/**
 * Initializes a tokens list structure with an allocated buffer.
 *
 * Note: the buffer size must be at least the size of the largest paseable
 * input + 1 (plus one!)
 */
int rp_cli_cmd_token_list_init(struct rp_cli_cmd_tokens *tokens,
        char *buf, uint_fast16_t buf_size);

/**
 * Parses the given input and fills out the tokens structure.
 *
 * Returns negative on error (one of RP_CLI_CMD_ERROR_* constants).
 */
int rp_cli_cmd_parse(struct rp_cli_cmd_tokens *tokens, const char *input);

#endif

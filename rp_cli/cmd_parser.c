#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "./cmd_parser.h"

/* macro to check if a character is a whitespace */
#define _IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')

/** Internal command line parser state machine */
enum _parser_state {
    CLI_PARSER_IGNORE = 0,
    CLI_PARSER_RECORD_ARG = 1,
    // future extensions:
    /* CLI_PARSER_ESCAPE = 0xE, */
    /* CLI_PARSER_IN_QUOTE = 0x10, */
};


/**
 * Initializes a tokens list structure with an allocated buffer.
 *
 * Note: the buffer size must be at least the size of the largest paseable
 * input + 1 (plus one!)
 */
int rp_cli_cmd_token_list_init(struct rp_cli_cmd_tokens *tokens,
        char *buf, uint_fast16_t buf_size)
{
    memset(tokens, 0, sizeof(struct rp_cli_cmd_tokens));
    tokens->_buf = buf;
    tokens->_buf_size = buf_size;
    return 0;
}

/**
 * Parses the given input and fills out the tokens structure.
 *
 * Returns negative on error (one of RP_CLI_CMD_ERROR_* constants).
 */
int rp_cli_cmd_parse(struct rp_cli_cmd_tokens *tokens, const char *input)
{
    int len = strlen(input);
    if ((len + 1) >= tokens->_buf_size)
        return -1;
    /* copy the input to the internal buffer (to be able to alter it) */
    memset(tokens->_buf, 0, len);
    tokens->argc = 0;
    int i, cur_buf_idx = 0, arg_buf_start = 0;
    enum _parser_state pstate = CLI_PARSER_IGNORE;

    for (i = 0; i<len; i++) {
        int cur_arg_finished = 0;
        char c = input[i];
_parse_repeat:
        switch (pstate) {
            case CLI_PARSER_IGNORE:
                if (_IS_WHITESPACE(c)) continue; /* keep ignoring */
                /* else: capture the argument */
                pstate = CLI_PARSER_RECORD_ARG;
                goto _parse_repeat;
            case CLI_PARSER_RECORD_ARG:
                if (_IS_WHITESPACE(c)) {
                    cur_arg_finished = 1;
                    pstate = CLI_PARSER_IGNORE;
                    break;
                }
                tokens->_buf[cur_buf_idx++] = c;
                /* reached the end? do not miss the argument! */
                if ((i + 1) >= len) cur_arg_finished = 1;
                break;
        }

        if (cur_arg_finished) {
            /* add null string terminator to the buffer */
            tokens->_buf[cur_buf_idx++] = '\0';
            tokens->argv[tokens->argc] = &tokens->_buf[arg_buf_start];
            arg_buf_start = cur_buf_idx;
            tokens->argc++;
        }
    }
    return tokens->argc;
}


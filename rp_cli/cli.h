#ifndef _LIB_RP_CLI_H
#define _LIB_RP_CLI_H

#ifdef RP_LIB_CONFIG_HEADER
#include RP_LIB_CONFIG_HEADER
#endif

#include "rp_util/preprocessor.h"

/** Import all CLI headers. */
#include "./readline.h"
#include "./cmd_parser.h"
#include "./cmd_registry.h"

#ifndef RP_CLI_DEF_LINE_BUF_SIZE
/** Maximum number of bytes allowed in a CLI line.
 * Note that this will be allocated twice (for line + tokenized args!). */
#define RP_CLI_DEF_LINE_BUF_SIZE  128
#endif

/** Enum with default CLI message/error codes. */
enum rp_cli_def_codes {
    RP_CLI_NOOP = 0,
    RP_CLI_SUCCESS = 1,
    RP_CLI_PARSE_FAILED = -1,
    RP_CLI_INVALID_CMD = -2,
    RP_CLI_HELP_MSG = 0xF1,
};

#define RP_CLI_STR_CODE(code)  ("[" STRINGIFY(code) "]")

/**
 * A ready to use descriptor structure integrating the default CLI modules.
 */
struct rp_cli_default_desc {
    /** Static readline descriptor. */
    struct rp_readline_desc rl;
    /** Pointer to the command registry (may be set afterwards). */
    const struct rp_cli_cmd_registry *reg;

    /** Internal line buffer. */
    char _rline_buf[RP_CLI_DEF_LINE_BUF_SIZE];
    /** Internal tokens structure */
    struct rp_cli_cmd_tokens _tokens;
    char _token_buf[RP_CLI_DEF_LINE_BUF_SIZE];
};

/**
 * Initialize a default CLI instance.
 */
int rp_cli_def_init(struct rp_cli_default_desc *cli,
                    const struct rp_cli_cmd_registry *registry,
                    const struct rp_cli_io_funcs *io_funcs);

/**
 * Default CLI processing routine: reads an input character, if a command line
 * is available, parse & execute it!
 *
 * Call regularly in your main loop.
 */
int rp_cli_def_process(struct rp_cli_default_desc *cli);

/**
 * Prints the default help message (for a given command or all of them).
 *
 * Only use with a default CLI instance!
 */
pico_cli_action_ret_t rp_cli_cmd_auto_help(
    int argc, const char *argv[], void *aux_data);

/**
 * Macro to add the default help command to the registry.
 *
 * The `p_def_cli` parameter must be a pointer to a valid `rp_cli_default_desc`!
 */
#define RP_CLI_HELP_CMD_ENTRY(p_def_cli)  \
    { "help", "help [COMMAND]: shows the help for all / single command", \
        NULL, rp_cli_cmd_auto_help, p_def_cli }

/** Macro to add dummy [CODE] error handler. */
#define RP_CLI_CODE_CMD_ENTRY(s_code, cb_handler)  \
    { s_code, "error handler for code " s_code, \
        NULL, cb_handler, NULL }

#endif

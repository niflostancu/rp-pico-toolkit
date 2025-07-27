#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "./cli.h"


/* Internal (static) routines & constants. */
static const struct rp_cli_cmd_entry *_find_command(
        const struct rp_cli_cmd_registry *registry, const char *cmd_token);
static int _print_msg(struct rp_cli_default_desc *cli, const char *code,
                      const char *msg);

static const char *_help_msgs[] = {
    "Commands available:",
    "Use 'help <command>' to get details about a command.",
};


/**
 * Initialize a default CLI instance.
 */
int rp_cli_def_init(struct rp_cli_default_desc *cli,
                    const struct rp_cli_cmd_registry *registry,
                    const struct rp_cli_io_funcs *io_funcs)
{
    /* initialize with default (static) buffers */
    rp_readline_init(&cli->rl, cli->_rline_buf, RP_CLI_DEF_LINE_BUF_SIZE, io_funcs);
    rp_cli_cmd_token_list_init(&cli->_tokens, cli->_token_buf, RP_CLI_DEF_LINE_BUF_SIZE);
    cli->reg = registry;
    return RP_CLI_SUCCESS;
}

/**
 * Default CLI processing routine: reads an input character, if a command line
 * is available, parse & execute it!
 *
 * Call regularly in your main loop.
 */
int rp_cli_def_process(struct rp_cli_default_desc *cli)
{
    char err_msg[100];
    int ret = RP_CLI_SUCCESS;
    int raw = cli->rl.io->read_char();
    if (raw < 0)
        return RP_CLI_NOOP;
    char c = (char)raw;
    enum rp_readline_state rstatus;
    rstatus = rp_readline_process(&cli->rl, c);
    if (rstatus == RP_READLINE_STATE_ERROR) {
        _print_msg(cli, RP_CLI_STR_CODE(RP_CLI_PARSE_FAILED),
                   "Readline error!");
    }
    if (rstatus != RP_READLINE_STATE_COMPLETE)
        return RP_CLI_NOOP;

    const char *cur_line = rp_readline_get_line_buffer(&cli->rl);
    int parse_ret = rp_cli_cmd_parse(&cli->_tokens, cur_line);
    if (parse_ret < 0) {
        _print_msg(cli, RP_CLI_STR_CODE(RP_CLI_PARSE_FAILED),
                   "Unable to parse input!");
        ret = RP_CLI_PARSE_FAILED;
        goto _reset_prompt;
    }
    if (cli->_tokens.argc < 1) {
        /* empty command string */
        ret = RP_CLI_NOOP;
        goto _reset_prompt;
    }
    const struct rp_cli_cmd_entry *cmd = _find_command(cli->reg, cli->_tokens.argv[0]);
    if (!cmd) {
        snprintf(err_msg, sizeof(err_msg), "Invalid command: `%s`. "
                 "Use 'help' for a list of commands available.",
                 cli->_tokens.argv[0]);
        _print_msg(cli, RP_CLI_STR_CODE(RP_CLI_PARSE_FAILED), err_msg);
        ret = RP_CLI_INVALID_CMD;
        goto _reset_prompt;
    }
    if (cmd->callback) {
        rp_cli_action_ret_t ret = cmd->callback(
            cli->_tokens.argc, cli->_tokens.argv, cmd->aux_data);
    }
_reset_prompt:
    rp_readline_reset(&cli->rl);
    return ret;
}

static const struct rp_cli_cmd_entry *_find_command(
        const struct rp_cli_cmd_registry *registry, const char *cmd_token)
{
    int i;
    for (i=0; i<registry->entries_count; i++) {
        if (strcmp(registry->entries[i].name, cmd_token) == 0) {
            return &registry->entries[i];
        }
    }
    return NULL;
}

/**
 * Internal routine to print a standard message on the CLI.
 *
 * Allows overriding the default action by registering special commands using
 * the `[CODE]` name convention. If not found, prints the default message
 * instead.
 */
static int _print_msg(struct rp_cli_default_desc *cli, const char *code,
                      const char *msg)
{
    const struct rp_cli_cmd_entry *cmd = NULL;
    if (code) {
        cmd = _find_command(cli->reg, code);
    }
    if (cmd) {
        const char *argv[] = {code, msg};
        rp_cli_action_ret_t ret = cmd->callback(2, argv, cmd->aux_data);
        if (ret < 0) return -1;
    } else {
        /* default message handler */
        if (!cli->rl.io->write(msg, strlen(msg)))
            return -1;
        if (!cli->rl.io->write(cli->rl.line_end, strlen(cli->rl.line_end)))
            return -1;
        cli->rl.io->flush_output();
    }
}

/** Prints the default help message (for a given command or all of them). */
rp_cli_action_ret_t rp_cli_cmd_auto_help(
    int argc, const char *argv[], void *aux_data)
{
    struct rp_cli_default_desc *cli = (struct rp_cli_default_desc *)aux_data;
    int i;
    char help_buf[256];

    /* TODO: implement command help */

    _print_msg(cli, RP_CLI_STR_CODE(RP_CLI_HELP_MSG), _help_msgs[0]);
    for (i=0; i<cli->reg->entries_count; i++) {
        snprintf(help_buf, sizeof(help_buf), "   %s: %s",
                 cli->reg->entries[i].name, cli->reg->entries[i].short_help);
        _print_msg(cli, NULL, help_buf);
    }

    return RP_CLI_CMD_BREAK;
}


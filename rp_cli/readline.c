#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "./readline.h"

/** Set to 1 to debug unknown control codes. */
#define _DEBUG_CODE  0

/** Default CLI prompt string. */
static const char *_default_prompt = "# ";


/**
 * Initializes the CLI library and registers the required callbacks.
 */
int rp_readline_init(struct rp_readline_desc *rline, char *line_buf,
                     int line_buf_size, const struct rp_cli_io_funcs* io_funcs)
{
    memset(rline, 0, sizeof(struct rp_readline_desc));
    rline->state = RP_READLINE_STATE_RESET;
    rline->line_buf = line_buf;
    rline->line_buf_size = line_buf_size;
    rline->line_buf_cur_pos = 0;
    rline->io = io_funcs;
    rline->prompt_str = _default_prompt;
    rline->line_end = "\r\n";
    return 0;
}

/**
 * Feeds one character to the CLI's state machine and manages
 * an equivalent readline process.
 *
 * Returns the final state (use COMPLETE for executing the command).
 */
enum rp_readline_state rp_readline_process(struct rp_readline_desc *rline, char c)
{
    if (rline->state == RP_READLINE_STATE_DISABLED)
        return RP_READLINE_STATE_DISABLED;
    if (rline->state == RP_READLINE_STATE_COMPLETE) {
        /* stay here until reset */
        return rline->state;
    }
    if (rline->state == RP_READLINE_STATE_RESET) {
        /* do a reset to display the initial prompt */
        rp_readline_reset(rline);
    }
    if (rline->state != RP_READLINE_STATE_INPUT)
        return rline->state;
    switch (c) {
        case '\r': /* carriage return: simply ignore */
        case '\0': /* NULL character or new line */
        case '\n':
            /** Command line is complete */
            rline->state = RP_READLINE_STATE_COMPLETE;
            if (c) {
                rline->io->write(rline->line_end, strlen(rline->line_end));
                rline->io->flush_output();
            }
            break;
        case '\b':   /* backspace */
        case '\x7f': /* delete */
            if (rline->line_buf_cur_pos > 0) {
                rline->line_buf[rline->line_buf_cur_pos - 1] = 0;
                rline->line_buf_cur_pos--;
                /* remove char from screen */
                rline->io->write("\b \b", 3);
                rline->io->flush_output();
            }
            break;

        default:
            /* only append displayable chars */
            if ((c >= 32 && c <= 126)) {
                /* append to buffer */
                if ((rline->line_buf_cur_pos + 1) >= rline->line_buf_size)
                    return RP_READLINE_STATE_ERROR;
                rline->line_buf[rline->line_buf_cur_pos++] = c;
                rline->line_buf[rline->line_buf_cur_pos] = 0;
                rline->io->write(&c, 1);
                rline->io->flush_output();
            } else {
#if defined(_DEBUG_CODE) && _DEBUG_CODE
                char _dbg_buf[8];
                int _dbg_n;
                _dbg_n = snprintf(_dbg_buf, sizeof(_dbg_buf), "<0x%02X>", c);
                rline->io->write(_dbg_buf, _dbg_n);
                rline->io->flush_output();
#endif
            }
            break;
    }
    return rline->state;
}

/**
 * Resets the readline prompt state (clears and re-renders it).
 *
 * Must manually call after processing a command to properly begin anew.
 */
enum rp_readline_state rp_readline_reset(struct rp_readline_desc *rline)
{

    if (rline->state == RP_READLINE_STATE_DISABLED)
        return RP_READLINE_STATE_DISABLED;

    /* reset CLI state */
    rline->line_buf[0] = 0;
    rline->line_buf_cur_pos = 0;
    if (rline->prompt_str)
        rline->io->write(rline->prompt_str, strlen(rline->prompt_str));
    rline->state = RP_READLINE_STATE_INPUT;
}

/**
 * Returns the current line buffer.
 */
const char *rp_readline_get_line_buffer(struct rp_readline_desc *rline)
{
    return rline->line_buf;
}


#ifndef _LIB_RP_CLI_READLINE_H
#define _LIB_RP_CLI_READLINE_H
/**
 * Embedded Readline library with line editing, history, autocompletion and
 * other features (WIP!).
 */

#include <stdint.h>
#include <stddef.h>

#include "./io.h"

/**
 * Represents the various states of the readline processing machine.
 *
 * The end user should simply check whether the line was COMPLETEd and process
 * the command in this case only.
 */
enum rp_readline_state {
    RP_READLINE_STATE_DISABLED = 0,
    RP_READLINE_STATE_RESET = 1,
    RP_READLINE_STATE_INPUT = 2,
    RP_READLINE_STATE_ESCAPE = 3,
    RP_READLINE_STATE_COMPLETE = 0xF,
    RP_READLINE_STATE_ERROR = -1,
};

/**
 * Descriptor structure encapsulating the configuration & state of
 * a Readline instance.
 */
struct rp_readline_desc {
    /** Current input state. */
    enum rp_readline_state state;
    /** Current prompt to display */
    const char *prompt_str;
    /** Line ending separator to use */
    const char *line_end;

    /** Pointer to structure containing I/O functions to use */
    const struct rp_cli_io_funcs *io;

    /** Current line contents, max size & current length. */
    char *line_buf;
    uint_fast16_t line_buf_size, line_buf_cur_pos;
};


/**
 * Initializes the CLI library and registers the required callbacks.
 */
int rp_readline_init(struct rp_readline_desc *rline, char *line_buf,
                     int line_buf_size, const struct rp_cli_io_funcs* io_funcs);

/**
 * Feeds one character to the CLI's state machine and manages
 * an equivalent readline process.
 *
 * Returns the final state (use COMPLETE when the final input is available).
 */
enum rp_readline_state rp_readline_process(struct rp_readline_desc *rline, char c);

/**
 * Resets the readline prompt state (clears and re-renders it).
 *
 * Must manually call after processing a command to properly begin anew.
 */
enum rp_readline_state rp_readline_reset(struct rp_readline_desc *rline);

/**
 * Returns the current line buffer.
 */
const char *rp_readline_get_line_buffer(struct rp_readline_desc *rline);

#endif

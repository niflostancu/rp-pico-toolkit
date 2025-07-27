#ifndef _LIB_RP_CLI_CMD_REGISTRY_H
#define _LIB_RP_CLI_CMD_REGISTRY_H
/**
 * Simple CLI command line registry & executor macros.
 */


struct rp_cli_cmd_registry;
struct rp_cli_cmd_entry;

/** RPI CLI action processing effects. */
typedef enum {
    /** Continue processing the next command in list. */
    RP_CLI_CMD_CONTINUE = 0,
    /** Current command was processed successfully, break from evaluation. */
    RP_CLI_CMD_BREAK = 1,
    /** Command execution failed (anything negative code be returned) */
    RP_CLI_CMD_ERROR = -1,

} rp_cli_action_ret_t;

/** Generic type of cooperative CLI command callback */
typedef rp_cli_action_ret_t (*rp_cli_cmd_calback_t)(
    int argc, const char *argv[], void *aux_data);

#define RP_CLI_STATIC_CMD(_NAME_)  \
    static rp_cli_action_ret_t _NAME_(int argc, const char *argv[], \
        void *aux_data)

/**
 * Simple CLI commands registry.
 */
struct rp_cli_cmd_registry {
    /** Array of simple command entries */
    const struct rp_cli_cmd_entry *entries;
    /** Number of entries in array. */
    int entries_count;
};

/** CLI command entry. */
struct rp_cli_cmd_entry {
    /** The command name / pattern to match. */
    const char *name;
    /** Short command help message. */
    const char *short_help;
    /** Full help string for this command (optional). */
    const char *description;
    /** Command callback. */
    rp_cli_cmd_calback_t callback;
    /** Auxilliary command data to supply to the callback. */
    void *aux_data;
};

#endif

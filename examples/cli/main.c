#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <tusb.h>
#if defined(RASPBERRYPI_PICO_W)
#include "pico/cyw43_arch.h"
#endif

#include "rp_util/cc_helpers.h"
#include "rp_cli/cli.h"
#include "rp_cli/rp_stdio.h"


/** Size of the CLI input & parser buffers. */
#define APP_CLI_BUF_SIZE  256

struct rp_cli_default_desc main_cli;

/** Command registry */
RP_CLI_STATIC_CMD(main_cmd_test);
RP_CLI_STATIC_CMD(main_cmd_hello);
const struct rp_cli_cmd_entry main_cli_commands[] = {
    RP_CLI_HELP_CMD_ENTRY(&main_cli),
    { "test", "test [NUMBER]: Runs the test command"
      "(optionally, specify a number).", NULL, main_cmd_test },
    { "hello", "hello NAME: prints a hello message", NULL, main_cmd_hello },
};

const struct rp_cli_cmd_registry main_reg = {
    .entries = main_cli_commands,
    .entries_count = RP_ARRAY_SIZE(main_cli_commands),
};

int main() {
    /* initialize Pico SDK stdio (USB / UART) */
    stdio_init_all();

    /* initialize RP CLI stdio library */
    rp_cli_def_init(&main_cli, &main_reg, &rp_cli_stdio_funcs);

    while (1) {
        /* only process CLI if the device was connected to a host */
        if (tud_cdc_connected()) {
            rp_cli_def_process(&main_cli);
        }
        tight_loop_contents();
    }
}

static rp_cli_action_ret_t main_cmd_test(int argc, const char *argv[],
        void *aux_data)
{
    long int n = 0;
    if (argc > 1) {
        n = strtol(argv[1], NULL, 10);
    }
    printf("Your number: n=%i\r\n", n);
    return 0;
}

static rp_cli_action_ret_t main_cmd_hello(int argc, const char *argv[],
        void *aux_data)
{
    const char *name = "<Anonymous>";
    if (argc > 1) {
        name = argv[1];
    }
    printf("Hello %s!\r\n", name);
    return 0;
}

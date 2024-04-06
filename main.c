#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include "prints.h"
#include "input.h"
#include "operations.h"

int main(void) {
    print_header();
    print_help(help);

    char cmd[LONGEST_CMD_STR + EXTRA_CHAR + STR_NULLC];
    char cmd_param_1[MAX_ALIAS_LEN + EXTRA_CHAR + STR_NULLC];
    char cmd_param_2[FILENAME_MAX + EXTRA_CHAR + STR_NULLC];

    enum command operation;
    do {
        do {
            print_prompt();
            operation = read_command(cmd, cmd_param_1, cmd_param_2);
        } while (operation == fail);
        operation = operate(operation, cmd_param_1, cmd_param_2);
    } while (operation != quit);
}
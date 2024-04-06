#include <stdio.h>
#include <string.h>
#include "prints.h"

void print_header(void) {
    for (int line_i = 0; line_i < HEADER_HEIGHT; line_i++) {
        // print top and bottom border
        if (line_i < HEADER_BOARDER || HEADER_HEIGHT - HEADER_BOARDER <= line_i) {
            for (int c_i = 0; c_i < HEADER_WIDTH; c_i++) {
                printf("%c", HEADER_BOARDER_CHAR);
            }
            printf("\n");
            // print side borders and padding (if line isn't the string line)
        } else if (line_i != HEADER_HEIGHT - HEADER_BOARDER - HEADER_PADDING - INDEX_OFFSET) {
            for (int c_i = 0; c_i < HEADER_WIDTH; c_i++) {
                printf("%c",
                       c_i < HEADER_BOARDER || HEADER_WIDTH - HEADER_BOARDER <= c_i ?
                       HEADER_BOARDER_CHAR : HEADER_PADDING_CHAR);
            }
            printf("\n");
            // print boarders, padding and header string
        } else {
            for (int c_i = 0; c_i < HEADER_WIDTH; c_i++) {
                if (c_i != HEADER_BOARDER + HEADER_PADDING) {
                    printf("%c",
                           c_i < HEADER_BOARDER || HEADER_WIDTH - HEADER_BOARDER <= c_i ?
                           HEADER_BOARDER_CHAR : HEADER_PADDING_CHAR);
                } else {
                    printf(HEADER_STRING);
                    c_i += strlen(HEADER_STRING) - INDEX_OFFSET;
                }
            }
            printf("\n");
        }
    }
}

void print_help(enum command cmd_enum) {
    printf("\n");
    if (cmd_enum == all) {
        for (int cmd_i = 0; cmd_i < CMD_COUNT; cmd_i++) {
            int dsc_i = 0;
            printf("%s\n", HELP_LIST[cmd_i][dsc_i]);
            ++dsc_i;
            while (strcmp(HELP_LIST[cmd_i][dsc_i], "0") != 0) {
                printf(" - %s\n", HELP_LIST[cmd_i][dsc_i]);
                ++dsc_i;
            }
        }
    } else {
        int dsc_i = 0;
        printf("%s\n", HELP_LIST[cmd_enum][dsc_i]);
        ++dsc_i;
        while (strcmp(HELP_LIST[cmd_enum][dsc_i], "0") != 0) {
            printf(" - %s\n", HELP_LIST[cmd_enum][dsc_i]);
            ++dsc_i;
        }
    }
    printf("\n");
}

void print_prompt(void) {
    printf("%s>", REPO_DIR_ADDRESS);
}
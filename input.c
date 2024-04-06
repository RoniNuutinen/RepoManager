#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "input.h"
#include "prints.h"

#define MAX_CMD_LEN (\
LONGEST_CMD_STR + EXTRA_CHAR\
+ MAX_ALIAS_LEN + EXTRA_CHAR\
+ FILENAME_MAX\
+ (INPUT_SPACE * 2) + STR_NULLC)

int get_word(const char * string, char * word, int max_word_len, int * index) {
    int w_i = 0;
    while (string[*index] != ' ' && string[*index] != '\0' && w_i < max_word_len) {
        word[w_i] = string[*index];
        ++w_i;
        ++*index;
    }
    word[w_i] = '\0';
    return w_i;
}

enum command read_command(char * cmd, char * cmd_param_1, char * cmd_param_2) {
    char input[MAX_CMD_LEN];
    fgets(input, MAX_CMD_LEN, stdin);
    input[strlen(input) - INDEX_OFFSET] = '\0';
    if (strlen(input) == 0) return fail;

    int max_len = LONGEST_CMD_STR + EXTRA_CHAR + STR_NULLC;
    int str_i = 0;
    get_word(input,
             cmd,
             max_len,
             &str_i);

    enum command operation;
    bool cmd_found = false;
    int cmd_i = 0;
    while (!cmd_found && cmd_i < CMD_COUNT) {
        if (strcmp(cmd, CMD_LIST[cmd_i]) == 0) {
            cmd_found = true;
            operation = cmd_i;
        }
        ++cmd_i;
    }
    if (!cmd_found) {
        fprintf(stderr, "Invalid Command: [%s...]\n", cmd);
        return fail;
    }
    // check if command is one of the two that don't take parameters
    switch (operation) {
        case list:
            return list;
        case quit:
            return quit;
        default:
            ;
    }
    ++str_i; // skip the space

    // help's parameter is <command> while others' is <alias>
    max_len = (operation == help ?
                LONGEST_CMD_STR :
                MAX_ALIAS_LEN) + EXTRA_CHAR + STR_NULLC;
    int para_1_len = get_word(input,
                              cmd_param_1,
                              max_len,
                              &str_i);
    // if no parameter found
    if (para_1_len == 0) {
        switch (operation) {
            case help:
                return help;
            case show:
                return show;
            default:
                fprintf(stderr,
                        "%s requires the first parameter\n", cmd);
                return fail;
        }
    } else if (para_1_len == max_len) {
        fprintf(stderr,
                "Too long parameter: [%s...]\n", cmd_param_1);
        return fail;
    }
    // add is the only one that requires 2 parameters
    switch (operation) {
        case help:
            return help;
        case show:
            return show;
        case delete:
            return delete;
        default:
            ;
    }
    ++str_i; // skip da space
    max_len = FILENAME_MAX - REPO_DIR_ADDRESS_LEN + EXTRA_CHAR + STR_NULLC;
    int para_2_len = get_word(input,
                              cmd_param_2,
                              max_len,
                              &str_i);
    if (para_2_len == 0) {
        fprintf(stderr,
                "Repository directory must have a filename.\n");
        return fail;
    } else if (para_2_len == max_len) {
            fprintf(stderr,
                    "Address can be a maximum of %d characters long\n",
                    FILENAME_MAX - REPO_DIR_ADDRESS_LEN);
            return fail;
    }
    return add;
}
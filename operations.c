#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <malloc.h>
#include <unistd.h>
#include "operations.h"
#include "prints.h"
#include "input.h"

#define MAX_LINE_LEN (MAX_ALIAS_LEN + INPUT_SPACE + FILENAME_MAX + STR_NULLC)

// executes help command
enum command call_help(const char * cmd_param_1) {
    if (cmd_param_1[0] == '\0') {
        print_help(all);
        return help;
    } else {
        int cmd_i = 0;
        enum command help_cmd = fail;
        while (help_cmd == fail && cmd_i < CMD_COUNT) {
            if (strcmp(cmd_param_1, CMD_LIST[cmd_i]) == 0) {
                help_cmd = cmd_i;
            }
            ++cmd_i;
        }
        if (help_cmd != fail) {
            print_help(help_cmd);
            return help;
        } else {
            fprintf(stderr, "Unknown command: [%s...]\n", cmd_param_1);
            return fail;
        }
    }
}

// check if file is "empty"
bool file_empty(FILE * file) {
    bool empty = false;
    if ((file = fopen(REPO_LINKS_ADDRESS, "r")) == NULL) {
        fprintf(stderr, "%s [%s]\n", strerror(errno), REPO_LINKS_ADDRESS);
        return false;
    }
    fgetc(file);
    if (feof(file)) empty = true;
    fclose(file);
    return empty;
}

// check if alias exists
bool alias_exists(FILE * repo_links, const char * alias_par) {
    bool alias_found = false;
    while (!feof(repo_links) && !alias_found) {
        char line[MAX_LINE_LEN];
        if (fgets(line, MAX_LINE_LEN, repo_links) != NULL) {
            int line_len = (int) strlen(line);
            line[line_len - INDEX_OFFSET] = '\0';
            if (line_len != 0) {
                int l_i = 0;
                char alias[MAX_ALIAS_LEN + STR_NULLC];
                get_word(line, alias, MAX_LINE_LEN, &l_i);
                if (strcmp(alias, alias_par) == 0) {
                    alias_found = true;
                }
            }
        }
    }
    return alias_found;
}

// pairs <alias> with <repo-address>
enum command add_link(const char * cmd_param_1, const char * cmd_param_2) {
    FILE * repo_links = NULL;
    bool empty = false;
    if (file_empty(repo_links)) empty = true;
    if ((repo_links = fopen(REPO_LINKS_ADDRESS, "r")) == NULL) {
        fprintf(stderr, "%s [%s]\n", strerror(errno), REPO_LINKS_ADDRESS);
        return fail;
    }
    if (alias_exists(repo_links, cmd_param_1)) {
        fclose(repo_links);
        fprintf(stderr, "Alias [%s] already reserved.\n", cmd_param_1);
        return fail;
    }
    fclose(repo_links);
    // check if address exists
    char repo_address[FILENAME_MAX + STR_NULLC] = REPO_DIR_ADDRESS;
    strcat(repo_address, cmd_param_2);
    DIR * repo = NULL;
    if ((repo = opendir(repo_address)) == NULL) {
        fprintf(stderr, "%s\n[%s]\n", strerror(errno), repo_address);
        closedir(repo);
        return fail;
    }
    closedir(repo);
    if ((repo_links = fopen(REPO_LINKS_ADDRESS, "a")) == NULL) {
        fprintf(stderr, "%s [%s]\n", strerror(errno), REPO_LINKS_ADDRESS);
        return fail;
    }
    if (!empty) fprintf(repo_links, "%c", '\n');
    fprintf(repo_links, "%s %s%s",
            cmd_param_1, REPO_DIR_ADDRESS, cmd_param_2);
    printf("\nAlias [%s] linked to\n%s%s\n\n", cmd_param_1, REPO_DIR_ADDRESS, cmd_param_2);
    fclose(repo_links);
    return add;
}

// prints all linked aliases with their addresses
enum command show_link(const char * cmd_param_1) {
    FILE * repo_links = NULL;
    printf("\n%*s -> %s\n", MAX_ALIAS_LEN, "Alias", "Repository Address");
    if (file_empty(repo_links)) {
        printf("%*s -> %s\n\n", MAX_ALIAS_LEN, "N/A", "N/A");
        return show;
    }
    if ((repo_links = fopen(REPO_LINKS_ADDRESS, "r")) == NULL) {
        fprintf(stderr, "%s [%s]\n", strerror(errno), REPO_LINKS_ADDRESS);
        return fail;
    }
    bool show_all = cmd_param_1[0] == '\0' ? true : false;
    while (!feof(repo_links)) {
        char line[MAX_LINE_LEN];
        if (fgets(line, MAX_LINE_LEN, repo_links) != NULL) {
            int line_len = (int) strlen(line);
            if (!feof(repo_links))
                line[line_len - INDEX_OFFSET] = '\0';
            if (line_len != 0) {
                char alias[MAX_ALIAS_LEN + STR_NULLC];
                char address[FILENAME_MAX + STR_NULLC];
                int l_i = 0;
                int alias_len = get_word(line,
                                         alias,
                                         MAX_ALIAS_LEN,
                                         &l_i);
                ++l_i; // skip ze space
                int address_len = get_word(line,
                                           address,
                                           FILENAME_MAX,
                                           &l_i);
                if (alias_len == 0) {
                    if (!feof(repo_links)) fgetc(repo_links);
                } else if (address_len == 0) {
                    fprintf(stderr, "Corrupted line: [%s...]\n", alias);
                } else {
                    if (show_all) {
                        printf("%*s -> %s\n", MAX_ALIAS_LEN, alias, address);
                    } else if (strcmp(alias, cmd_param_1) == 0) {
                        printf("%*s -> %s\n", MAX_ALIAS_LEN, alias, address);
                        fclose(repo_links);
                        printf("\n");
                        return show;
                    }
                }
            }
        }
    }
    fclose(repo_links);
    if (!show_all) {
        fprintf(stderr, "Alias [%s] not found\n", cmd_param_1);
        return fail;
    }
    printf("\n");
    return show;
}

// for linked char list
typedef struct char_link {
    char c;
    struct char_link *next;
} new_char;

// adds a new node to the linked list
bool link_new_char(new_char ** link, char c) {
    new_char * next_char = (struct char_link *) malloc(sizeof(struct char_link));
    if (next_char == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        return false;
    }
    (*link)->c = c;
    (*link)->next = next_char;
    next_char->c = '\0';
    next_char->next = NULL;
    *link = next_char;
    return true;
}

// saves <line> (string) into <linked_list>
bool link_line(const char * line, new_char ** link) {
    int line_i = 0;
    while (line[line_i] != '\0') {
        if (!link_new_char(link, line[line_i])) {
            return false;
        }
        ++line_i;
    }
    return true;
}

// write the <linked list> of characters to <file>
void append_linked_list(FILE * file, new_char * link) {
    do {
        // no null && no newline if it is the last character
        if (link->c != '\0' && (link->c != '\n' || (link->next)->c != '\0')) {
            fprintf(file, "%c", link->c);
        }
        new_char * prev_node = link;
        link = link->next;
        free(prev_node);
    } while (link != NULL);
}

// deletes alias link (including address) from repo_links file
enum command delete_alias(const char * cmd_param_1) {
    FILE * repo_links = NULL;
    if (file_empty(repo_links)) {
        fprintf(stderr, "File [%s] empty\n", REPO_LINKS_ADDRESS);
        return fail;
    }
    if ((repo_links = fopen(REPO_LINKS_ADDRESS, "r")) == NULL) {
        fprintf(stderr, "%s [%s]\n", strerror(errno), REPO_LINKS_ADDRESS);
        return fail;
    }
    if (!alias_exists(repo_links, cmd_param_1)) {
        fprintf(stderr, "Alias [%s] not found\n", cmd_param_1);
        fclose(repo_links);
        return fail;
    }
    fclose(repo_links);
    if ((repo_links = fopen(REPO_LINKS_ADDRESS, "r")) == NULL) {
        fprintf(stderr, "%s [%s]\n", strerror(errno), REPO_LINKS_ADDRESS);
        return fail;
    }
    new_char * linked_str_start = (struct char_link *) malloc(sizeof(struct char_link));
    linked_str_start->next = NULL;
    linked_str_start->c = '\0';
    new_char * linked_str = linked_str_start;

    while (!feof(repo_links)) {
        char line[MAX_LINE_LEN];
        if (fgets(line, MAX_LINE_LEN, repo_links) != NULL) {
            char alias[MAX_ALIAS_LEN + STR_NULLC];
            int l_i = 0;
            get_word(line,
                     alias,
                     MAX_ALIAS_LEN + STR_NULLC,
                     &l_i);
            if (strcmp(alias, cmd_param_1) != 0) {
                if (!link_line(line, &linked_str)) {
                    fclose(repo_links);
                    return fail;
                }
            } else {
                ++l_i;
                char address[FILENAME_MAX + STR_NULLC];
                get_word(line, address, FILENAME_MAX, &l_i);
                address[strlen(address) - INDEX_OFFSET] = '\0';
                printf("\nDeleted link [%s -> %s]\n\n", alias, address);
            }
        }
    }
    fclose(repo_links);
    if (remove(REPO_LINKS_ADDRESS) != 0) {
        fprintf(stderr, "%s [%s]\n", strerror(errno), REPO_LINKS_ADDRESS);
        return fail;
    }
    if ((repo_links = fopen(REPO_LINKS_ADDRESS, "a")) == NULL) {
        fprintf(stderr, "%s [%s]\n", strerror(errno), REPO_LINKS_ADDRESS);
        return fail;
    }
    append_linked_list(repo_links, linked_str_start);
    fclose(repo_links);
    return delete;
}

// pick operation
enum command operate(enum command operation, const char * cmd_param_1, const char * cmd_param_2) {
    switch (operation) {
        case help:
            call_help(cmd_param_1);
            return help;
        case add:
            add_link(cmd_param_1, cmd_param_2);
            return add;
        case show:
            show_link(cmd_param_1);
            return show;
        case delete:
            delete_alias(cmd_param_1);
            return delete;
        case list:
            show_link("");
            return list;
        case quit:
            printf("\nAre you sure? [y/n]\n");
            sleep(2);
            printf("... just kidding\n");
            sleep(1);
            return quit;
        default:
            fprintf(stderr, "Something weird happened..\n");
            return fail;
    }
}

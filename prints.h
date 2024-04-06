#define INDEX_OFFSET 1

#define REPO_DIR_ADDRESS "C:/Users/ronin/CLionProjects/"
#define REPO_DIR_ADDRESS_LEN (int) strlen(REPO_DIR_ADDRESS)
#define REPO_LINKS_ADDRESS "../repo_links.txt"

#define HEADER_STRING "Repository Manager"
#define HEADER_BOARDER 1
#define HEADER_BOARDER_CHAR '#'
#define HEADER_PADDING 1
#define HEADER_PADDING_CHAR ' '
#define HEADER_WIDTH (strlen(HEADER_STRING) + (HEADER_BOARDER + HEADER_PADDING) * 2)
#define HEADER_HEIGHT (1 + (HEADER_PADDING + HEADER_BOARDER) * 2)

#define HELP "help"
#define ADD "add"
#define SHOW "show"
#define LIST "list"
#define QUIT "quit"
#define DELETE "delete"

#define CMD_COUNT 6
#define LONGEST_CMD_STR 6

#define STR_NULLC 1

// can now be called with enum command as well
#define CMD_LIST (char [CMD_COUNT] [LONGEST_CMD_STR + STR_NULLC] )\
{HELP, ADD, SHOW, LIST, QUIT, DELETE}

enum command {
    help,
    add,
    show,
    list,
    quit,
    delete,
    all,
    fail
};

#define HELP_DESC \
{"help <command>",\
"Describes the function of <command>",\
"Omitting <command> lists all command descriptions",\
"0"}
#define ADD_DESC \
{"add <alias> <repository link>",\
"Links a new alias to an existing repository address",\
"<alias> must be vacant, max 20 characters long and without spaces",\
"link address must be found in repository directory anc can ",\
"0"}
#define SHOW_DESC \
{"show <alias>",\
"Show the corresponding link(s) to alias(es)",\
"Omitting <alias> shows all aliases",\
"0"}
#define LIST_DESC \
{"list",\
"Lists all aliases in use",\
"0"}
#define QUIT_DESC \
{"quit",\
"Terminates the program",\
"0"}
#define DELETE_DESC \
{"delete <alias>",\
"Deletes the <alias> -link",\
"0"}

// desc as in description
#define MAX_DESC_LINES 4
#define MAX_DESC_LINE_LEN 100

// list of all help descriptions
#define HELP_LIST (char [CMD_COUNT][MAX_DESC_LINES + STR_NULLC][MAX_DESC_LINE_LEN])\
{HELP_DESC, ADD_DESC, SHOW_DESC, LIST_DESC, QUIT_DESC, DELETE_DESC}

// prints a fancy startup header,
// margin and padding adjustable
void print_header(void);

// print command description(s)
void print_help(enum command cmd_enum);

// print 'shell prompt'
void print_prompt(void);

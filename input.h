// for checking if input is too long,
// instead of possibly cutting it short before checking
#define EXTRA_CHAR 1

#define MAX_ALIAS_LEN 20
// the space that separates <command> <param> <param>
// from user input
#define INPUT_SPACE 1

// reads a word into <word> from a <string>
int get_word(const char * string, char * word, int max_word_len, int * index);
// reads input from user,
// returns operation according to input, assuming input is valid
enum command read_command(char * cmd, char * cmd_param_1, char * cmd_param_2);

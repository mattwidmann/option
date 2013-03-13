#ifndef OPTION_H
#define OPTION_H

typedef int (*option_callback_t)(char * argument, void * user_data);

typedef struct option {
    option_callback_t on_option; /* callback called when option is parsed */
    void * user_data; /* passed to callback */
    char * long_name; /* name to use for --option style options */
    char * description; /* description for option (for help message) */
    char * argument_name; /* explanation of argument, set to NULL for no argument */

    char short_name; /* name to use for -o style options */
} option_t;

typedef struct option_list {
    option_callback_t on_unknown_option;
    option_callback_t on_literal_argument;

    option_t * options;
    void * user_data; // passed to error callbacks
} option_list_t;

/* prints a list of available options with descriptions, across multiple lines */
void options_print(option_list_t * options);
/* prints an abbreviated list of available options */
void options_print_inline(option_list_t * options);

/* returns 0 on success, non-zero to indicate where error occurred in argument list */
int options_parse(option_list_t *options, int argc, char * * argv);

#endif


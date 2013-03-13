#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/option.h"

int unknown(char * argument, void * data)
{
    printf("unrecognized %d: %s\n", *(int *)data, argument);

    return 0;
}

int literal(char * argument, void * data)
{
    printf("literal %d: %s\n", *(int *)data, argument);

    return 0;
}

int plain(char * argument, void * data)
{
    printf("plain\n");

    return 0;
}

int arg(char * argument, void * data)
{
    printf("saw %s\n", argument);

    return 0;
}

int main(int argc, char *argv[])
{
    option_callback_t on_option; /* callback called when option is parsed */
    void * user_data; /* passed to callback */
    char * long_name; /* name to use for --option style options */
    char * description; /* description for option (for help message) */
    char * argument_name; /* explanation of argument, set to NULL for no argument */

    char short_name; /* name to use for -o style options */
    option_t list[] = {{.on_option = plain, .user_data = NULL, .long_name = "plain", .description = "I'm plain", .argument_name = NULL, .short_name = 'p'}, {.on_option = arg, .user_data = NULL, .long_name = "arg", .description = "I'm an arg", .argument_name = "argument", .short_name = 'a'}, {.on_option = NULL}};
    option_list_t option_list = {.on_unknown_option = &unknown, .on_literal_argument = &literal, .options = list};
    options_process(&option_list, argc, argv);
}

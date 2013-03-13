#include "option.h"

#include <stdio.h>
#include <string.h>

static int parse_short(option_list_t * options, int argc, char * * argv, int i);
static int parse_long(option_list_t * options, int argc, char * * argv, int i);

int options_process(option_list_t * options, int argc, char * * argv)
{
    // skip first argument
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') { // is an option
            int new_index;
            if (argv[i][1] == '-') { // long option
                if (argv[i][2] == '\0') { // rest of arguments are literal
                    while (++i < argc) {
                        if (options->on_literal_argument(argv[i], options->user_data))
                            return i;
                    }

                    return 0;
                }

                new_index = parse_long(options, argc, argv, i);
            } else { // short option
                new_index = parse_short(options, argc, argv, i);
            }

            // couldn't parse an option, but started with '-'
            if (!new_index) {
                if (options->on_unknown_option(argv[i], options->user_data))
                    return i;

                new_index = i + 1;
            }

            i = new_index - 1;;
        } else { // literal argument
            if (options->on_literal_argument(argv[i], options->user_data))
                return i;
        }
    }

    return 0;
}

static int parse_short(option_list_t * options, int argc, char * * argv, int i)
{
    int arg_len = strlen(argv[i]);

    // skip leading '-'
    for (int j = 1; j < arg_len; j++) {
        // process rest of options in argument
        int option_index = -1;
        for (int k = 0; options->options[k].on_option; ++k) {
            if (options->options[k].short_name == argv[i][j]) {
                option_index = k;
            }
        }

        if (option_index == -1) { // unknown option
            return 0;
        }

        option_t * option = options->options + option_index;

        if (option->argument_name) { // option takes an argument
            char * argument = NULL;

            if (j == (arg_len - 1)) { // this is the last flag in the argument
                if (i + 1 < argc) { // the next argument exists
                    argument = argv[++i];
                }
            } else { // treat rest of argument as option's argument (-fargument)
                argument = argv[i] + j + 1;
            }

            option->on_option(argument, option->user_data);
            break;
        } else {
            option->on_option(NULL, option->user_data);
        }

    }

    return i + 1;
}

static int parse_long(option_list_t * options, int argc, char * * argv, int i)
{
    for (int j = 0; options->options[j].on_option; j++) {
        option_t * option = options->options + j;
        if (!option->long_name) {
            continue;
        }

        int option_len = strlen(option->long_name);
        int arg_len = strlen(argv[i] + 2);

        // argument is smaller than the option or the two don't match
        if (arg_len < option_len || strncmp(option->long_name, argv[i] + 2, option_len)) {
            continue;
        }

        char * argument = NULL;
        if (option->argument_name) {
            if (arg_len != option_len) { // argument continues after the long name has matched
                if (argv[i][option_len + 2] != '=') { // the first character of that continuation is not '='
                    continue;
                }

                argument = argv[i] + option_len + 3;
            } else {
                if (i + 1 < argc) { // the next argument exists
                    argument = argv[++i];
                }
            }
        }

        option->on_option(argument, option->user_data);
        return i + 1;
    }

    // no options matched
    return 0;
}

void options_print(option_list_t *options)
{
    int option_index = 0;
    option_t * option = options->options + option_index;

    // while (option->callback != NULL) {
        // return;
    // }
}

void options_print_inline(option_list_t *options)
{
    char line[80];
    int options_left = 1;
    int option_index = 0;
    while (options_left) {
        option_index++;
        return;
    }
    /* char *test = "grep [-abcdDEFGHhIiJLlmnOopqRSsUVvwxZ] [-A num] [-B num] [-C[num]] \ */
    /*       [-e pattern] [-f file] [--binary-files=value] [--color[=when]] \ */
    /*       [--colour[=when]] [--context[=num]] [--label] [--line-buffered] [--null] \ */
    /*       [pattern] [file ...]" */
}


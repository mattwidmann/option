#include "option.h"

#include <stdio.h>
#include <string.h>

static int option_for_short_argument(option_list_t *options, char flag);
static int parse_input(option_list_t *options, int option, int argc,
                       char *argv[], int i);
static int parse_short_name(option_list_t *options, int argc, char *argv[],
                            int i);
static int parse_long_name(option_list_t *options, int argc, char *argv[],
                           int i);

#define CHECK_CALLBACK(Argument, Index) do { \
    if (option->callback != NULL && \
        option->callback(options, Index, Argument) == 0) return 0; } while (0)

#define CHECK_ERROR_CALLBACK(Callback, Option) do { \
    if (options->Callback != NULL && \
        options->Callback(options, Option, argv[i], i) == 0) return 0; } \
    while (0)

int options_parse(option_list_t *options, int argc,
                  char *argv[])
{
    char *program_name = argv[0];

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int result;
            if (argv[i][1] == '-') {
                if (strlen(argv[i]) == 2) {
                    // rest of arguments are literal
                    return i + 1;
                }

                result = parse_long_name(options, argc, argv, i);
            } else {
                result = parse_short_name(options, argc, argv, i);
            }

            if (result == 0) {
                return i;
            }

            i = result;
        } else {
            CHECK_ERROR_CALLBACK(unknown_option_callback, -1);
        }
    }

    return argc;
}

static int parse_short_name(option_list_t *options, int argc, char *argv[],
                            int i)
{
    int argument_length = strlen(argv[i]);

    for (int j = 1; j < argument_length; j++) {
        int option_index = option_for_short_argument(options, argv[i][j]);
        if (option_index == -1) {
            CHECK_ERROR_CALLBACK(unknown_option_callback, -1);
            continue;
        }

        option_t *option = options->options + option_index;
        if (j == (argument_length - 1)) {
            if (option->option_input == SINGLE_OPTION_INPUT ||
                option->option_input == OPTIONAL_OPTION_INPUT) {
                // treat next option as possible argument
                return parse_input(options, option_index, argc, argv, i);
            }

            CHECK_CALLBACK(NULL, option_index);
        } else if (option->option_input == SINGLE_OPTION_INPUT) {
            // treat rest of flag as argument (-fargument)
            CHECK_CALLBACK(argv[i] + j + 1, option_index);
            break;
        // option takes no arguments
        } else CHECK_CALLBACK(NULL, option_index);
    }

    return i;
}

static int parse_long_name(option_list_t *options, int argc, char *argv[],
                           int i)
{
    for (int j = 0; options->options[j].callback != NULL; j++) {
        option_t *option = options->options + j;
        if (option->long_name == NULL) {
            continue;
        }

        if (strcmp(option->long_name, argv[i] + 2) == 0) {
            return parse_input(options, j, argc, argv, i);
        }

        // matches up to = and can take argument
        if (strncmp(option->long_name, argv[i] + 2,
                    strlen(option->long_name)) == 0 &&
            argv[i][strlen(option->long_name) + 2] == '=' &&
            (option->option_input == SINGLE_OPTION_INPUT ||
             option->option_input == OPTIONAL_OPTION_INPUT)) {
            // treat rest of option as argument (--option=argument)
            CHECK_CALLBACK(argv[i] + strlen(option->long_name) + 3, j);
            return i;
        }
    }

    CHECK_ERROR_CALLBACK(unknown_option_callback, -1);

    return i;
}

static int option_for_short_argument(option_list_t *options, char argument)
{
    for (int i = 0; options->options[i].callback != NULL; i++) {
        if (options->options[i].short_name == argument) {
            return i;
        }
    }

    return -1;
}

static int parse_input(option_list_t *options, int option_index, int argc,
                       char *argv[], int i)
{
    option_t *option = options->options + option_index;
    int inputs_left = argc - (i + 1);
    if (option->option_input == NO_OPTION_INPUT) {
        CHECK_CALLBACK(NULL, option_index);
    } else if (option->option_input == SINGLE_OPTION_INPUT) {
        if (inputs_left > 0) {
            CHECK_CALLBACK(argv[i + 1], option_index);
            return i + 1;
        }

        CHECK_ERROR_CALLBACK(missing_argument_callback, option_index);
    } else if (option->option_input == REST_OPTION_INPUT) {
        if (inputs_left > 0) {
            for (int j = i + 1; j < argc; j++) {
                CHECK_CALLBACK(argv[j], option_index);
            }

            return i + inputs_left;
        }

        CHECK_ERROR_CALLBACK(missing_argument_callback, option_index);
    } else if (option->option_input == OPTIONAL_OPTION_INPUT) {
        if (inputs_left > 0 && argv[i + 1][0] != '-') {
            CHECK_CALLBACK(argv[i + 1], option_index);
            return i + 1;
        }

        CHECK_CALLBACK(NULL, option_index);
    }

    return i;
}

void options_print(option_list_t *options)
{
    int option_index = 0;
    option_t *option = options->options[option_index];

    while (option->callback != NULL) {
    }
}

void options_print_inline(option_list_t *options)
{
    char line[80];
    int options_left = 1;
    int option_index = 0;
    while (options_left) {
        option_index++;
    }
    /* char *test = "grep [-abcdDEFGHhIiJLlmnOopqRSsUVvwxZ] [-A num] [-B num] [-C[num]] \ */
    /*       [-e pattern] [-f file] [--binary-files=value] [--color[=when]] \ */
    /*       [--colour[=when]] [--context[=num]] [--label] [--line-buffered] [--null] \ */
    /*       [pattern] [file ...]" */
}


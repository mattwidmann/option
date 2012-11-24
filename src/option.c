#include "option.h"

#include <stdio.h>
#include <string.h>

static const option_t *option_for_short_argument(const option_list_t *options,
                                  const char flag);
static void callback(const option_t *option, const char *argument);
static int parse_input(const option_t *option, const int argc,
                       const char *argv[], const int i);
static inline int parse_short_name(const option_list_t *options, const int argc,
                                   const char *argv[], const int i);
static inline int parse_long_name(const option_list_t *options, int argc,
                                  const char *argv[], int i);

int options_parse(const option_list_t *options, const int argc,
                  const char *argv[])
{
    const char *program_name = argv[0];

    const option_t *option;
    for (int i = 1; i < argc; i++) {
        int argument_length = strlen(argv[i]);

        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                if (argument_length == 2) {
                    return i + 1;
                }

                i = parse_long_name(options, argc, argv, i);
            } else {
                i = parse_short_name(options, argc, argv, i);
            }
        } else {
            // unrecognized argument
            options->unrecognized_option_callback(option, argv[i]);
        }
    }

    return argc;
}

static inline int parse_short_name(const option_list_t *options, const int argc,
                                   const char *argv[], const int i)
{
    int argument_length = strlen(argv[i]);
    for (int j = 1; j < argument_length; j++) {
        const option_t *option = option_for_short_argument(options, argv[i][j]);
        if (option == NULL) {
            // unrecognized flag
            options->unrecognized_option_callback(option, argv[i]);
        } else if (j == (argument_length - 1)) {
            // last flag
            return parse_input(option, argc, argv, i);
        } else if (option->option_input == SINGLE_OPTION_INPUT) {
            // treat rest of flag as argument
            callback(option, argv[i] + j + 1);
            break;
        } else {
            callback(option, NULL);
        }
    }

    return i;
}

static inline int parse_long_name(const option_list_t *options, const int argc,
                                  const char *argv[], const int i)
{
    const option_t *option;
    for (int j = 0; options->options[j].callback != NULL; j++) {
        option = &(options->options[j]);
        if (option->long_name != NULL) {
            if (strcmp(option->long_name, argv[i] + 2) == 0) {
                return parse_input(option, argc, argv, i);
            } else if (strncmp(option->long_name, argv[i] + 2,
                       strlen(option->long_name)) == 0 &&
                        argv[i][strlen(option->long_name) + 2] == '=' &&
                       (option->option_input == SINGLE_OPTION_INPUT ||
                        option->option_input == OPTIONAL_OPTION_INPUT)) {
                callback(option, argv[i] + strlen(option->long_name) + 3);
                return i;
            }
        }
    }

    // unrecognized argument
    return i + 1;
}

static const option_t *option_for_short_argument(const option_list_t *options,
                                                 const char argument)
{
    int i;
    for (i = 0; options->options[i].callback != NULL; i++) {
        if (options->options[i].short_name == argument) {
            return &(options->options[i]);
        }
    }

    return NULL;
}

static int parse_input(const option_t *option, const int argc,
                       const char *argv[], const int i)
{
    int j;
    int inputs_left = argc - (i + 1);
    if (option->option_input == NO_OPTION_INPUT) {
        callback(option, NULL);
    } else if (option->option_input == SINGLE_OPTION_INPUT) {
        if (inputs_left > 0) {
            callback(option, argv[i + 1]);
            return i + 1;
        }

        printf("no argument\n");
    } else if (option->option_input == REST_OPTION_INPUT) {
        if (inputs_left > 0) {
            for (int j = i + 1; j < argc; j++) {
                callback(option, argv[j]);
            }

            return i + inputs_left;
        }

        printf("NO ARGS\n");
    } else if (option->option_input == OPTIONAL_OPTION_INPUT) {
        if (inputs_left > 0 && argv[i + 1][0] != '-') {
            callback(option, argv[i + 1]);
            return i + 1;
        }

        callback(option, NULL);
    } else {
        printf("no argument!\n");
    }

    return i;
}

static void callback(const option_t *option, const char *argument)
{
    if (option->callback(option, argument) == 0) {
    }
}

int options_usage(const option_t *option, const char *arguments)
{
    char flags[10];
    printf("prog name: %s\n", (char *)option->user_data);
    /* char *test = "grep [-abcdDEFGHhIiJLlmnOopqRSsUVvwxZ] [-A num] [-B num] [-C[num]] \ */
    /*       [-e pattern] [-f file] [--binary-files=value] [--color[=when]] \ */
    /*       [--colour[=when]] [--context[=num]] [--label] [--line-buffered] [--null] \ */
    /*       [pattern] [file ...]" */
    return 1;
}

int options_help(const option_t *option, const char *arguments)
{
    puts("help");
    return 1;
}

int options_version(const option_t *option, const char *arguments)
{
    puts("version");
    return 1;
}

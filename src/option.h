#ifndef OPTION_H
#define OPTION_H

#define OPTION(LongName, ShortName, Description, OptionInput, Callback, UserData) \
{ \
    .long_name = LongName, \
    .short_name = ShortName, \
    .description = Description, \
    .option_input = OptionInput, \
    .callback = Callback, \
    .user_data = UserData, \
}

#define OPTION_USAGE(ProgramName) \
{ \
    .long_name = "usage", \
    .short_name = '\0', \
    .description = "print a consolidated argument listing", \
    .option_input = NO_OPTION_INPUT, \
    .callback = &options_usage, \
    .user_data = (void *)ProgramName, \
}

#define OPTION_HELP(ProgramName) \
{ \
    .long_name = "help", \
    .short_name = 'h', \
    .description = "print this message", \
    .option_input = NO_OPTION_INPUT, \
    .callback = &options_help, \
    .user_data = (void *)ProgramName, \
}

#define OPTION_VERSION(ProgramVersion) \
{ \
    .long_name = "version", \
    .short_name = 'v', \
    .description = "print version information", \
    .option_input = NO_OPTION_INPUT, \
    .callback = &options_version, \
    .user_data = (void *)ProgramVersion, \
}

#define OPTION_LAST \
{ \
    .long_name = NULL, \
    .short_name = '\0', \
    .description = NULL, \
    .option_input = NO_OPTION_INPUT, \
    .callback = NULL, \
    .user_data = NULL, \
}

typedef struct option option_t;

typedef int (*option_callback_t)(const option_t *option, const char *argument);

typedef enum option_input {
    NO_OPTION_INPUT,
    SINGLE_OPTION_INPUT,
    OPTIONAL_OPTION_INPUT,
    REST_OPTION_INPUT,
} option_input_t;

typedef struct option {
    char *long_name;
    char short_name;
    char *description;
    option_input_t option_input;
    option_callback_t callback;
    void *user_data;
} option_t;

typedef struct option_list {
    option_callback_t unrecognized_option_callback;
    option_t *options;
} option_list_t;

int options_parse(const option_list_t *options, const int argc, const char *argv[]);

int options_usage(const option_t *option, const char *argument);
int options_help(const option_t *option, const char *argument);
int options_version(const option_t *option, const char *argument);

#endif

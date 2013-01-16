#ifndef OPTION_H
#define OPTION_H

#define OPTION(LongName, ShortName, Description, ArgumentName, OptionType, \
               OptionInput, Callback, UserData) \
{ \
    .long_name = LongName, \
    .short_name = ShortName, \
    .description = Description, \
    .argument_name = ArgumentName, \
    .option_type = OptionType, \
    .option_input = OptionInput, \
    .callback = Callback, \
    .user_data = (void *)UserData, \
}

#define OPTION_END OPTION(NULL, '\0', NULL, NULL, END_OPTION_TYPE, \
                          NO_OPTION_INPUT, NULL, NULL)

struct option_list; // forward declaration for callback typedef

typedef int (*option_callback_t)(struct option_list *options, int option,
                                 char *argument);
typedef int (*option_error_callback_t)(struct option_list *options, int option,
                                       char *argument, int number);

typedef enum option_type {
    END_OPTION,
    REQUIRED_OPTION,
    OPTIONAL_OPTION,
} option_type_t;

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
    char *argument_name;
    option_type_t option_type;
    option_input_t option_input;
    option_callback_t callback;
    void *user_data;
} option_t;

typedef struct option_list {
    option_error_callback_t unknown_option_callback;
    option_error_callback_t missing_argument_callback;
    option_t *options;
} option_list_t;

int options_parse(option_list_t *options, int argc, char *argv[]);
void options_print(options_list_t *options);
void options_print_inline(options_list_t *options);

#endif

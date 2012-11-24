#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option.h"

int on_long(void *data, const char *arguments)
{
    printf("saw a long\n");

    return 1;
}

int on_short(void *data, const char *arguments)
{
    printf("saw a short\n");

    return 1;
}

int unrecognized(const option_t *option, const char *argument)
{
    printf("unrecognized\n");

    return 1;
}

int arg(const option_t *option, const char *argument)
{
    printf("saw %s\n", argument);

    return 1;
}

int main(int argc, const char *argv[])
{
    char *vers = "1.2.3";

    option_t list[] = {OPTION_HELP(argv[0]), OPTION_USAGE(argv[0]), OPTION("arg", 'a', "takes an argument", SINGLE_OPTION_INPUT, &arg, NULL), OPTION_LAST};
    option_list_t option_list = {.unrecognized_option_callback = &unrecognized, .options = list};
    options_parse(&option_list, argc, argv);
}

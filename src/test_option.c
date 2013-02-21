#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option.h"

int unknown(option_list_t *options, int option, char *argument, int i)
{
    printf("unrecognized %d: %s\n", i, argument);

    return 1;
}

int missing(option_list_t *options, int option, char *argument, int i)
{
    printf("missing %d: %s\n", i, argument);

    return 1;
}

int arg(option_list_t *options, int option, char *argument)
{
    printf("saw %s\n", argument);

    return 1;
}

int main(int argc, char *argv[])
{
    char *vers = "1.2.3";

    option_t list[] = {OPTION("arg", 'a', "takes an argument", "argument", REQUIRED_OPTION_TYPE, SINGLE_OPTION_INPUT, &arg, NULL), OPTION_END};
    option_list_t option_list = {.unknown_option_callback = &unknown, .missing_argument_callback = &missing, .options = list};
    options_parse(&option_list, argc, argv);
}

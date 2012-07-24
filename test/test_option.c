#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option.h"

int maxStringLength(const char *first, const char *second)
{
    int firstLength = strlen(first), secondLength = strlen(second);
    if (firstLength >= secondLength) {
        return firstLength;
    }

    return secondLength;
}

int equalStrings(const char *first, const char *second)
{
    return (strncmp(first, second, maxStringLength(first, second)) == 0);
}

struct payload {
    int sawLong;
    int sawShort;
};

int onLong(void *data, const char *arguments[], int size)
{
    ((struct payload *)data)->sawLong = 1;
    printf("saw a long\n");

    return 1;
}

int onShort(void *data, const char *arguments[], int size)
{
    ((struct payload *)data)->sawShort = 1;
    printf("saw a short\n");

    return 1;
}

int main(int argc, const char *argv[])
{
    struct payload payload = {
        .sawLong = 0,
        .sawShort = 0,
    };

    char *vers = "1.2.3";

    Option options[] = {
        OPTION("test", 't', "test the option parser", NoOptionInput, &onShort, &payload),
        OPTION_USAGE(options),
        OPTION_HELP(options),
        OPTION_VERSION(vers),
        OPTION_LAST,
    };

    //optionAdd(&options, optionInit(NULL, "long", "l", "tests the long name functionality", kFlagOptionType, kNoOptionInput, &onLong, &payload));
    //optionAdd(&options, optionInit(NULL, "short", "s", "tests the short name functionality", kFlagOptionType, kNoOptionInput, &onShort, &payload));
    optionsParse(options, argc, argv);

    return 0;
}

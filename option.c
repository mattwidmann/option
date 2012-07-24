#include "option.h"

#include <string.h>

static const Option *optionsParseFlag(const Option *options, const char flag);
static void callback(const Option *option, const char *arguments[], const int size);
static int optionParseInput(const Option *option, const int argc, const char *argv[], const int i);

void optionsParse(const Option *options, const int argc, const char *argv[])
{
    char *commandName = (char *)argv[0];

    int i, j, found;
    Option *option;
    int argumentLength;
    for (i = 1; i < argc; i += 1) {
        argumentLength = strlen(argv[i]);

        if (argumentLength == 2 && strncmp(argv[i], "--", 2) == 0) {
            // the rest of the arguments are to be taken literally as arguments to the program
        } else if (argumentLength > 2 && strncmp(argv[i], "--", 2) == 0) {
            found = 0;
            for (j = 0; options[j].callback != NULL; j += 1) {
                if (options[j].longName != NULL && equalStrings(options[j].longName, &(argv[i][2]))) {
                    found = 1;
                    i = optionParseInput(&(options[j]), argc, argv, i);
                    break;
                }
            }

            if (found == 0) {
                // unrecognized option
            }
        } else if (argumentLength > 1 && strncmp(argv[i], "-", 1) == 0) {
            for (j = 1; j < argumentLength; j += 1) {
                option = (Option *)optionsParseFlag(options, argv[i][j]);
                if (option != NULL) {
                    // last flag
                    if (j == argumentLength - 1) {
                        i = optionParseInput(option, argc, argv, i);
                    } else if (option->optionInput == SingleOptionInput || option->optionInput == MultipleOptionInput) {
                        // requires argument, but immediately preceded by another flag
                    } else {
                        callback(option, NULL, 0);
                    }
                } else {
                    // unrecognized flag
                }
            }
        } else {
            // unrecognized option
        }
    }
}

static const Option *optionsParseFlag(const Option *options, const char flag)
{
    int i;
    for (i = 0; options[i].callback != NULL; i += 1) {
        if (options[i].shortName == flag) {
            return &(options[i]);
        }
    }

    return NULL;
}

static int optionParseInput(const Option *option, const int argc, const char *argv[], const int i)
{
    int j;
    int inputsLeft = argc - (i + 1);
    switch (option->optionInput) {
        case NoOptionInput:
            callback(option, NULL, 0);
            return i;
            break;
        case SingleOptionInput:
            if (inputsLeft > 0) {
                callback(option, argv + (i + 1), 1);
                return i + 1;
            } else {
                // missing argument
            }

            break;
        case MultipleOptionInput:
            if (inputsLeft > 0) {
                callback(option, argv + (i + 1), inputsLeft);
                return i + inputsLeft;
            } else {
                // missing arguments
            }

            break;
        case OptionalOptionInput:
            if (inputsLeft > 0 && argv[i + 1][0] == '-') {
                callback(option, argv + (i + 1), 1);
                return i + 1;
            } else {
                callback(option, NULL, 0);
                return i;
            }

            break;
        default:
            // bad option input value
            break;
    }
}

static void callback(const Option *option, const char *arguments[], const int size)
{
    if (option->callback(option->userData, arguments, size) == 0) {
        // option callback failure
    }
}

int optionsUsage(void *options, const char *arguments[], const int size)
{

}

int optionsHelp(void *options, const char *arguments[], const int size)
{

}

int optionVersion(void *version, const char *arguments[], const int size)
{

}

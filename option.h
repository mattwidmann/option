#ifndef OPTION_H
#define OPTION_H

#define OPTION(theLongName, theShortName, theDescription, theOptionInput, theCallback, theUserData)\
{\
    .longName = theLongName,\
    .shortName = theShortName,\
    .description = theDescription,\
    .optionInput = theOptionInput,\
    .callback = theCallback,\
    .userData = theUserData,\
}

#define OPTION_USAGE(options) \
{\
    .longName = "usage",\
    .shortName = '\0',\
    .description = "print a consolidated argument listing to stdout",\
    .optionInput = NoOptionInput,\
    .callback = &optionsUsage,\
    .userData = options,\
}

#define OPTION_HELP(options) \
{\
    .longName = "help",\
    .shortName = 'h',\
    .description = "print this message to stdout",\
    .optionInput = NoOptionInput,\
    .callback = &optionsHelp,\
    .userData = options,\
}

#define OPTION_VERSION(programVersion) \
{\
    .longName = "version",\
    .shortName = 'v',\
    .description = "print version information to stdout",\
    .optionInput = NoOptionInput,\
    .callback = &optionVersion,\
    .userData = programVersion,\
}

#define OPTION_LAST \
{\
    .longName = "",\
    .shortName = '\0',\
    .description = "",\
    .optionInput = NoOptionInput,\
    .callback = NULL,\
    .userData = NULL,\
}

typedef enum OptionInput {
    NoOptionInput,
    SingleOptionInput,
    OptionalOptionInput,
    MultipleOptionInput,
} OptionInput;

typedef struct Option {
    char *longName;
    char shortName;
    char *description;
    OptionInput optionInput;
    int (*callback)(void *userData, const char *arguments[], const int size);
    void *userData;
} Option;

void optionsParse(const Option *options, const int argc, const char *argv[]);
int optionsHelp(void *data, const char *arguments[], const int size);
int optionVersion(void *data, const char *arguments[], const int size);
int optionsUsage(void *data, const char *arguments[], const int size);

#endif

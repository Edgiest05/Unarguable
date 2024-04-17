# Unarguable
> Managing command line arguments will lead to no more disagreements

![Issues](https://img.shields.io/github/issues/Edgiest05/Unarguable?label=Issues)
![Pull requests](https://img.shields.io/github/issues-pr/Edgiest05/Unarguable?label=Pull%20requests)
![GitHub License](https://img.shields.io/github/license/Edgiest05/Unarguable?label=License)
![GitHub Release](https://img.shields.io/github/v/release/Edgiest05/Unarguable?label=Latest%20Release)

This repository provides a **single source file** to seamlessly manage command line arguments, compliant with the most widely compatible [C90 standard](https://en.wikipedia.org/wiki/ANSI_C#C90).

Using a `UA_Parser` one can define needed arguments and manage their traits (simple flag, required argument, multiple parameters, etc) and ultimately check if parsing is successful and retrieve stored values.

# Example

```c
#include <stdio.h>

#define UNARGUABLE_IMPLEMENTATION
#include "unarguable.h"

int main(int argc, char const *argv[]) {
    UA_Parser *parser = ua_create_parser();
    UA_Argument *arg;
    UA_ArgValue *argValue;
    const char *wrongArg = NULL;

    ua_parser_add_argument(parser, UA_ARGUMENT_BOTH, "s", "save", 1, true);
    ua_parser_populate_arguments(parser, argc, argv);

    arg = ua_parser_get_argument(parser, "s");
    UA_PRINT_ARGUMENT(arg, "s");

    if ((wrongArg = ua_parser_is_complete(parser)) != NULL) {
        fprintf(stderr, "First wrong argument is: '%s'\n", wrongArg);
        return 1;
    }

    argValue = ua_parser_get_argvalue(parser, arg);
    printf("Argument --save is: %s\n", argValue->items[0]);

    return 0;
}
```
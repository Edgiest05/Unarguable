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
#include <stdio.h>

#define UNARGUABLE_IMPLEMENTATION
#include "unarguable.h"

int main(int argc, const char *argv[]) {
    UA_Parser *parser = ua_parser_create();
    UA_Argument *arg;
    UA_ArgValues *argValue;
    const char *wrongArg = NULL;

    ua_parser_add_simple_argument(parser);
    ua_parser_add_argument(parser, UA_ARGUMENT_BOTH, "s", "save", 1, true);
    ua_parser_populate_arguments(parser, argc, argv);

    arg = ua_parser_get_argument(parser, "s");
    UA_PRINT_ARGUMENT(arg, "s");
    arg = ua_parser_get_simple_argument(parser, 0);
    UA_PRINT_ARGUMENT(arg, "0");

    if ((wrongArg = ua_parser_is_complete(parser)) != NULL) {
        fprintf(stderr, "First wrong argument is: '%s'\n", wrongArg);
        return 1;
    }

    argValue = ua_argument_get_values(arg);
    printf("First simple argument is: %s\n", argValue->items[0]);

    return 0;
}
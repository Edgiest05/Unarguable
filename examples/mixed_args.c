#include <stdio.h>

#define UNARGUABLE_IMPLEMENTATION
#include "unarguable.h"

int main(int argc, char const *argv[]) {
    UA_Parser *parser = ua_create_parser();
    UA_Argument *arg;
    const char *wrongArg = NULL;

    ua_parser_add_argument(parser, UA_ARGUMENT_BOTH, "s", "save", 1, true);
    ua_parser_add_argument(parser, UA_ARGUMENT_SHORT, "a", NULL, 0, true);
    ua_parser_add_argument(parser, UA_ARGUMENT_SHORT, "b", NULL, 0, false);
    ua_parser_add_argument(parser, UA_ARGUMENT_SHORT, "c", NULL, 0, false);
    ua_parser_add_argument(parser, UA_ARGUMENT_LONG, NULL, "directory", 1, false);
    ua_parser_populate_arguments(parser, argc, argv);

    arg = ua_parser_get_argument(parser, "s");
    UA_PRINT_ARGUMENT(arg, "s");
    UA_PRINT_ARGUMENT_VALUE(arg, "s");
    arg = ua_parser_get_argument(parser, "save");
    UA_PRINT_ARGUMENT(arg, "save");
    UA_PRINT_ARGUMENT_VALUE(arg, "save");
    arg = ua_parser_get_argument(parser, "a");
    UA_PRINT_ARGUMENT(arg, "a");
    UA_PRINT_ARGUMENT_VALUE(arg, "a");
    arg = ua_parser_get_argument(parser, "directory");
    UA_PRINT_ARGUMENT(arg, "directory");
    UA_PRINT_ARGUMENT_VALUE(arg, "directory");
    
    if ((wrongArg = ua_parser_is_complete(parser)) != NULL) {
        fprintf(stderr, "First wrong argument is: '%s'\n", wrongArg);
    }
    return 0;
}

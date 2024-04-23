#ifndef UNARGUABLE_H
#define UNARGUABLE_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define UA_ARR_LEN(array) (sizeof(array) / sizeof(array[0]))

/*  Dynamic Arrays */

#define UA_DA_INIT_CAP 4

#define UA_DA_APPEND(daPtr, elem)                                                                  \
    do {                                                                                           \
        if ((daPtr)->allocated >= (daPtr)->capacity) {                                             \
            (daPtr)->capacity = (daPtr)->capacity == 0 ? UA_DA_INIT_CAP : (daPtr)->capacity * 2;   \
            (daPtr)->items = realloc((daPtr)->items, (daPtr)->capacity * sizeof(*(daPtr)->items)); \
            assert((daPtr)->items != NULL && "Buy more RAM lol");                                  \
        }                                                                                          \
                                                                                                   \
        (daPtr)->items[(daPtr)->allocated++] = (elem);                                             \
    } while (0)

#define UA_DA_APPEND_MANY(daPtr, new_items, new_items_count)                                               \
    do {                                                                                                   \
        if ((daPtr)->allocated + new_items_count > (daPtr)->capacity) {                                    \
            if ((daPtr)->capacity == 0) {                                                                  \
                (daPtr)->capacity = UA_DA_INIT_CAP;                                                        \
            }                                                                                              \
            while ((daPtr)->allocated + new_items_count > (daPtr)->capacity) {                             \
                (daPtr)->capacity *= 2;                                                                    \
            }                                                                                              \
            (daPtr)->items = realloc((daPtr)->items, (daPtr)->capacity * sizeof(*(daPtr)->items));         \
            assert((daPtr)->items != NULL && "Buy more RAM lol");                                          \
        }                                                                                                  \
        memcpy((daPtr)->items + (daPtr)->allocated, new_items, new_items_count * sizeof(*(daPtr)->items)); \
        (daPtr)->allocated += new_items_count;                                                             \
    } while (0)

#define UA_DA_CLEAR(daPtr) (daPtr)->allocated = 0

#define UA_DA_FREE(daPtr) free((daPtr)->items)

/*  HashMaps */

#define UA_HM_BUCKET_SIZE 512

size_t ua_hash_string(const char *const str) {
    const char *cur = str;
    size_t hash = 5381;
    int c;
    while ((c = *cur++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/*  End of structures definition */

#define UAAPI

typedef enum UA_Argument_Type {
    UA_ARGUMENT_SHORT,
    UA_ARGUMENT_LONG,
    UA_ARGUMENT_BOTH
} UA_Argument_Type;

/*  typedef enum UA_Argument_DataType {
        UA_ARGUMENT_NUMBER_TYPE,
        UA_ARGUMENT_STRING_TYPE
    } UA_Argument_DataType; */

typedef struct UA_ArgValues {
    const char **items;
    size_t allocated;
    size_t capacity;
    bool isActive;
} UA_ArgValues;

typedef struct UA_Argument {
    UA_Argument_Type type;
    char *shortName;
    char *longName;
    size_t consumes;
    UA_ArgValues values;
    bool isRequired;
} UA_Argument;

typedef struct _UA_LL_Argument_Map _UA_LL_Argument_Map;
struct _UA_LL_Argument_Map {
    const char *key; /*  Lives in the argument stack */
    size_t value;
    _UA_LL_Argument_Map *next;
};

typedef struct UA_Parser {
    /*  Map name -> arg stack address -> arg value stack address */
    struct UA_Argument_Stack {
        UA_Argument *items;
        size_t allocated;
        size_t capacity;
    } argumentStack;

    /*  From string of argument name to index of argument in argument stack */
    struct UA_Argument_Map {
        _UA_LL_Argument_Map *buckets[UA_HM_BUCKET_SIZE];
    } argumentMap;
} UA_Parser;

#define UA_PRINT_ARGUMENT(argPtr, id) \
    printf("ARGUMENT(%s) -> type: %d, short: %s, long: %s, consumes: %lu, required: %d\n", (id), (argPtr)->type, (argPtr)->shortName, (argPtr)->longName, (unsigned long)(argPtr)->consumes, (argPtr)->isRequired)

#define UA_PRINT_ARGUMENT_VALUE(argPtr, id) \
    printf("VALUE(%s) -> found: %d, consumes: %lu, length: %lu\n", (id), (argPtr)->values.isActive, (unsigned long)(argPtr)->consumes, (unsigned long)(argPtr)->values.allocated)

UAAPI UA_Parser *ua_parser_create();
UAAPI void ua_parser_add_argument(UA_Parser *parser, UA_Argument_Type argType, const char *const shortName, const char *const longName, size_t elementsCounsumed, bool isArgRequired);
UAAPI UA_Argument *ua_parser_get_argument(UA_Parser *parser, const char *const name);
UAAPI bool ua_parser_populate_arguments(UA_Parser *parser, int argc, const char *const argv[]); /* // ! Only supports prefixed identifiers (-, --) */
UAAPI const char *ua_parser_is_complete(const UA_Parser *parser);                               /* // TODO: Does this signature make sense? */

UAAPI void ua_argument_set_consumes(UA_Argument *argument, size_t consumes);
UAAPI void ua_argument_set_required(UA_Argument *argument, bool isRequired);
UAAPI UA_ArgValues *ua_argument_get_values(UA_Argument *argument);
UAAPI void ua_argument_set_active(UA_Argument *argument, bool isActive);

#ifdef UNARGUABLE_IMPLEMENTATION
UA_Parser *ua_parser_create() {
    return calloc(1, sizeof(UA_Parser));
}

void _ua_parser_map_add_helper(UA_Parser *parser, const char *const keyPtr, size_t value) {
    /*  ! Needs to be sure that the key is not already present before-hand */
    size_t idx = ua_hash_string(keyPtr) % UA_HM_BUCKET_SIZE;
    _UA_LL_Argument_Map **cur = &parser->argumentMap.buckets[idx];
    while (*cur != NULL) {
        *cur = (*cur)->next;
    }
    (*cur) = calloc(1, sizeof(_UA_LL_Argument_Map));
    (*cur)->key = keyPtr;
    (*cur)->value = value;
}

void _ua_parser_map_add(UA_Parser *parser, UA_Argument_Type argType, const char *const shortName, const char *const longName, size_t argumentIdx) {
    if (argType != UA_ARGUMENT_LONG) {
        _ua_parser_map_add_helper(parser, shortName, argumentIdx);
    }

    if (argType != UA_ARGUMENT_SHORT) {
        _ua_parser_map_add_helper(parser, longName, argumentIdx);
    }
}

size_t _ua_parser_map_find(UA_Parser *parser, const char *const name) {
    _UA_LL_Argument_Map *cur = parser->argumentMap.buckets[ua_hash_string(name) % UA_HM_BUCKET_SIZE];
    while (cur != NULL && strcmp(name, cur->key) != 0) {
        cur = cur->next;
    }
    return (cur != NULL && strcmp(cur->key, name) == 0) ? cur->value : SIZE_MAX;
}

void ua_parser_add_argument(UA_Parser *parser, UA_Argument_Type argType, const char *const shortName, const char *const longName, size_t elementsCounsumed, bool isArgRequired) {
    size_t shortNameSize, longNameSize;
    char *newShortName = NULL, *newLongName = NULL;
    UA_Argument new = {0};

    if (argType != UA_ARGUMENT_LONG) {
        shortNameSize = strlen(shortName) + 1;
        newShortName = calloc(shortNameSize, sizeof(*newShortName));
        memcpy(newShortName, shortName, shortNameSize * sizeof(*newShortName));
        assert(shortNameSize - 1 == 1 && "Invalid short name length");
    }

    if (argType != UA_ARGUMENT_SHORT) {
        longNameSize = strlen(longName) + 1;
        newLongName = calloc(longNameSize, sizeof(*newLongName));
        memcpy(newLongName, longName, longNameSize * sizeof(*newLongName));
        assert(longNameSize - 1 > 1 && "Invalid long name length");
    }

    new.type = argType;
    new.shortName = newShortName;
    new.longName = newLongName;
    new.consumes = elementsCounsumed;
    new.isRequired = isArgRequired;
    UA_DA_APPEND(&parser->argumentStack, new);

    _ua_parser_map_add(parser, argType, newShortName, newLongName, parser->argumentStack.allocated - 1);
}

UA_Argument *ua_parser_get_argument(UA_Parser *parser, const char *const name) {
    size_t argIdx = _ua_parser_map_find(parser, name);
    return argIdx == SIZE_MAX ? NULL : &parser->argumentStack.items[argIdx];
}

void ua_argument_set_consumes(UA_Argument *argument, size_t consumes) {
    argument->consumes = consumes;
}

void ua_argument_set_required(UA_Argument *argument, bool isRequired) {
    argument->isRequired = isRequired;
}

UA_ArgValues *ua_argument_get_values(UA_Argument *argument) {
    return &argument->values;
}

void ua_argument_set_active(UA_Argument *argument, bool isActive) {
    ua_argument_get_values(argument)->isActive = isActive;
}

/* // ! Only supports prefixed identifiers (-, --) */
bool ua_parser_populate_arguments(UA_Parser *parser, int argc, const char *const argv[]) {
    /* // TODO: Handle argument redefinition (warn or error) */
    UA_Argument *cur = NULL;
    char compoundBuffer[2] = {0};

    int i;
    size_t j, _, curStrLen;
    i = 1;
    while (i < argc) {
        curStrLen = strlen(argv[i]); /* NULL terminator is excluded */
        if (curStrLen == 0 || argv[i][0] != '-') return false;
        if (argv[i][1] != '-') {
            /* Check if compound */
            if (curStrLen > 2) {
                for (j = 1; j < curStrLen; j++) {
                    compoundBuffer[0] = argv[i][j];
                    cur = ua_parser_get_argument(parser, compoundBuffer);
                    if (cur == NULL) return false;
                    /* // TODO: Argument consumption is not possible with compund */
                    if (cur->consumes > 0) return false;
                    ua_argument_get_values(cur)->isActive = true;
                }
                i++;
                continue;
            }
            cur = ua_parser_get_argument(parser, argv[i] + 1);
        } else {
            cur = ua_parser_get_argument(parser, argv[i] + 2);
        }
        if (cur == NULL) {
            printf("[Warning] Undefined argument %s\n", argv[i]);
            i++;
            continue;
        }
        ua_argument_get_values(cur)->isActive = true;
        i++;
        for (_ = 0; _ < cur->consumes; _++) {
            if (i >= argc) return false;
            UA_DA_APPEND(ua_argument_get_values(cur), argv[i]);
            i++;
        }
    }
    return true;
}

const char *ua_parser_is_complete(const UA_Parser *parser) {
    UA_Argument *cur = NULL;
    UA_ArgValues *curValue = NULL;

    size_t i;
    for (i = 0; i < parser->argumentStack.allocated; i++) {
        cur = &parser->argumentStack.items[i];
        curValue = ua_argument_get_values(cur);
        if (cur->isRequired && !curValue->isActive) goto defer;
        if (curValue->isActive && (cur->consumes != curValue->allocated)) goto defer;
    }
    return NULL;

defer:
    return (cur->type == UA_ARGUMENT_SHORT) ? cur->shortName : cur->longName;
}

#endif /* UNARGUABLE_IMPLEMENTATION */

#endif /* UNARGUABLE_H */
#include "../include/z_string.h"

#include <error.h>
#include <stdlib.h>
#include <utils.h>

String * create_str() {
    String *str = malloc(sizeof(String));
    str->capacity = STRING_INIT_CAP;
    str->size = 0;
    str->data = malloc(STRING_INIT_CAP);
    str->data[0] = '\0';

    return str;
}

void free_str(String *str) {
    if (str && str->data) free(str->data);
    if (str) free(str);
}

void str_push_back(String *str, char c) {
    if (str->size + 1 >= str->capacity) {
        str->capacity *= 2;
        void *new_ptr = xrealloc(str->data, str->capacity);
        str->data = new_ptr;
    }

    str->data[str->size] = c;
    str->data[str->size + 1] = '\0';
    str->size++;
}

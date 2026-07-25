#include "../include/z_string.h"

#include <error.h>
#include <stdlib.h>
#include <utils.h>

String * create_str() {
    String *str = malloc(sizeof(String));
    str->capacity = STRING_INIT_CAP;
    str->size = 0;
    str->data = malloc(16);

    return str;
}

void free_str(String *str) {
    free(str->data);
    free(str);
}

void push_back(String *str, char c) {
    if (str->size >= str->capacity) {
        str->capacity *= 2;
        void *new_ptr = xrealloc(str->data, str->capacity);
        str->data = new_ptr;
    }

    str->data[str->size] = c;
    str->size++;
}

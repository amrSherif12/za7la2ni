#include "../include/string.h"

#include <stdlib.h>

String * create_str() {
    String *str = malloc(sizeof(String));
    str->capacity = 16;
    str->size = 0;
    str->data = malloc(16);

    return str;
}

void push_back(String *str, char c) {
    if (str->size >= str->capacity) {
        str->capacity *= 2;
        void *new_ptr = realloc(str->data, str->capacity);

        if (new_ptr == NULL) exit(1);
        str->data = new_ptr;
    }

    str->data[str->size] = c;
    str->size++;
}

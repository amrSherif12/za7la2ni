#ifndef STRING_H
#define STRING_H
#include <stddef.h>

#define STRING_INIT_CAP 16

typedef struct String String;

struct String {
    char *data;
    size_t size;
    size_t capacity;
};

String *create_str();

void free_str(String *str);

void str_push_back(String *str, char c);

#endif //STRING_H

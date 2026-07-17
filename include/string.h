#ifndef STRING_H
#define STRING_H
#include <stddef.h>

typedef struct String String;

struct String {
    char *data;
    size_t size;
    size_t capacity;
};

String *create_str();

void push_back(String *str, char c);

#endif //STRING_H

#ifndef MAP_H
#define MAP_H
#include <stddef.h>


typedef struct Map Map;

struct Map {
    char *entries;
    size_t capacity;
    size_t entry_size;
    size_t type_size;
};

Map *create_map(size_t capacity, size_t entry_size);

void hash(Map *map, const char *key, void *value);

void *find(Map *map, const char *key);

#endif //MAP_H

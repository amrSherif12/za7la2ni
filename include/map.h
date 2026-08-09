#ifndef MAP_H
#define MAP_H
#include <stddef.h>
#include <stdint.h>


typedef struct Map Map;

struct Map {
    char *entries;
    size_t capacity;
    size_t entry_size;
    size_t type_size;
};

Map *create_map(size_t capacity, size_t type_size);

void free_map(Map *map);

void map_hash(Map *map, const char *key, uint8_t key_len, void *value);

void *map_find(Map *map, const char *key, uint8_t key_len);

#endif //MAP_H

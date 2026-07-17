#include "../include/map.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

Map *create_map(size_t capacity, size_t type_size) {
    Map *map = malloc(sizeof(Map));
    map->capacity = capacity;
    map->entry_size = sizeof(char *) + type_size + sizeof(bool);
    map->type_size = type_size;
    map->entries = calloc(capacity, sizeof(map->entry_size));

    return map;
}

static size_t fnv1a(const char *key, size_t capacity) {
    uint32_t hash = 2166136261u;
    while (*key) {
        hash ^= (uint8_t) *key;
        hash *= 16777619u;
        key++;
    }
    return hash % capacity;
}


void hash(Map *map, const char *key, void *value) {
    size_t idx = fnv1a(key, map->capacity);
    char *entry = map->entries + idx * map->entry_size;
    bool *ent_act = (bool *) entry + sizeof(char *) + map->type_size;

    while (*ent_act) {
        if (key == *(char**)entry) break;
        if (++idx >= map->capacity) idx = 0;

        entry = map->entries + idx * map->entry_size;
        ent_act = (bool *) entry + sizeof(char *) + map->type_size;
    }

    memcpy((char**)entry, key, sizeof(char*));
    void *ent_val = entry + sizeof(char *);

    *ent_act = true;
    memcpy(ent_val, value, map->type_size);
}

void *find(Map *map, const char *key) {
    size_t idx = fnv1a(key, map->capacity);
    char *entry = map->entries + idx * map->entry_size;
    bool *ent_act = (bool *) entry + sizeof(char *) + map->type_size;
    if (!*ent_act) return NULL;

    while (key == *(char**)entry) {
        if (++idx >= map->capacity) idx = 0;
        entry = map->entries + idx * map->entry_size;
    }

    return entry + sizeof(char*);
}

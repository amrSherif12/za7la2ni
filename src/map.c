#include "../include/map.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <utils.h>

Map *create_map(size_t capacity, size_t type_size) {
    Map *map = xmalloc(sizeof(Map));
    map->capacity = capacity;
    size_t unaligned_size = sizeof(char *) + type_size + sizeof(bool) + sizeof(uint8_t);
    map->entry_size = (unaligned_size + 7) & ~7;
    map->type_size = type_size;
    map->entries = xcalloc(capacity, map->entry_size);

    return map;
}

void free_map(Map *map) {
    if (map && map->entries) free(map->entries);
    if (map) free(map);
}

static size_t fnv1a(const char *key, uint8_t key_len, size_t capacity) {
    uint32_t hash = 2166136261u;
    for (uint8_t i = 0; i < key_len; i++) {
        hash ^= (uint8_t) key[i];
        hash *= 16777619u;
    }
    return hash % capacity;
}

void map_hash(Map *map, const char *key, uint8_t key_len, void *value) {
    size_t idx = fnv1a(key, key_len, map->capacity);
    char *entry = (char*)map->entries + idx * map->entry_size;
    bool *ent_act = (bool *)(entry + sizeof(char *) + map->type_size);
    uint8_t *key_len_ptr = (uint8_t *)(entry + sizeof(char *) + map->type_size + sizeof(bool));
    int cnt = 0;

    while (*ent_act && cnt <= map->capacity) {
        if (key == *(char**)entry || (*key_len_ptr == key_len && strncmp(key, *(char**)entry, key_len) == 0)) {
            break;
        }

        if (++idx >= map->capacity) idx = 0;
        cnt++;

        entry = (char*)map->entries + idx * map->entry_size;
        ent_act = (bool *)(entry + sizeof(char *) + map->type_size);
        key_len_ptr = (uint8_t *)(entry + sizeof(char *) + map->type_size + sizeof(bool));
    }

    *(char**)entry = (char*)key;
    *key_len_ptr = key_len;
    *ent_act = true;
    memcpy(entry + sizeof(char *), value, map->type_size);
}

void *map_find(Map *map, const char *key, uint8_t key_len) {
    size_t idx = fnv1a(key, key_len, map->capacity);
    char *entry = (char*)map->entries + idx * map->entry_size;
    bool *ent_act = (bool *)(entry + sizeof(char *) + map->type_size);
    uint8_t *key_len_ptr = (uint8_t *)(entry + sizeof(char *) + map->type_size + sizeof(bool));
    int cnt = 0;

    while (*ent_act && cnt <= map->capacity) {
        if (key == *(char**)entry || (*key_len_ptr == key_len && strncmp(key, *(char**)entry, key_len) == 0)) {
            return entry + sizeof(char*);
        }

        if (++idx >= map->capacity) idx = 0;
        cnt++;

        entry = (char*)map->entries + idx * map->entry_size;
        ent_act = (bool *)(entry + sizeof(char *) + map->type_size);
        key_len_ptr = (uint8_t *)(entry + sizeof(char *) + map->type_size + sizeof(bool));
    }

    return NULL;
}
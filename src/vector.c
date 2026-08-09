#include "../include/vector.h"

#include <error.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

Vector * create_vector(size_t capacity, size_t type_size) {
    if (capacity < 8) capacity = 8;

    Vector *vector = xmalloc(sizeof(Vector));
    vector->capacity = capacity;
    vector->size = 0;
    vector->type_size = type_size;
    vector->data = xcalloc(capacity, vector->type_size);

    return vector;
}

void free_vector(Vector *vector) {
    if (vector && vector->data) free(vector->data);
    if (vector) free(vector);
}

void vec_push_back(Vector *vector, void *value) {
    if (vector->size >= vector->capacity) {
        vector->capacity *= 2;
        vector->data = xrealloc(vector->data, vector->capacity * vector->type_size);
    }

    char *entry = vector->data + vector->size * vector->type_size;
    memcpy(entry, value, vector->type_size);
    vector->size++;
}

void vec_pop_back(Vector *vector) {
    if (vector->size == 0) internal_error("vec_pop_back on an empty vector");
    vector->size--;
}

void * vec_back(Vector *vector) {
    if (vector->size == 0) return NULL;
    return vector->data + vector->type_size * (vector->size - 1);
}

void *vec_find(Vector *vector, size_t idx) {
    if (idx >= vector->size) internal_error("vec_find out of bound idx.");
    return vector->data + vector->type_size * idx;
}

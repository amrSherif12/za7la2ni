#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct Vector Vector;

struct Vector {
    char *data;
    size_t size;
    size_t capacity;
    size_t type_size;
};

Vector *create_vector(size_t capacity, size_t type_size);

void free_vector(Vector *vector);

void vec_push_back(Vector *vector, void *value);

void vec_pop_back(Vector *vector);

void *vec_back(Vector *vector);

void *vec_find(Vector *vector, size_t idx);

#endif //VECTOR_H

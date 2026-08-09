#include <gtest/gtest.h>

extern "C" {
    #include "vector.h"
}

TEST(Vector, AddsEntry) {
    Vector *vector = create_vector(8, sizeof(int));

    int x = 10;
    vec_push_back(vector, &x);

    EXPECT_EQ(vector->size, 1);
    EXPECT_EQ(*(int*)vector->data, x);

    free_vector(vector);
}

TEST(Vector, FindsEntry) {
    Vector *vector = create_vector(8, sizeof(int));

    EXPECT_EQ(vec_back(vector), nullptr);

    for (int i = 0; i < 8; i++) {
        vec_push_back(vector, &i);
    }

    EXPECT_EQ(vector->size, 8);
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(*(int*)vec_find(vector, i), i);
    }

    EXPECT_EQ(*(int*)vec_back(vector), 7);

   free_vector(vector);
}

TEST(Vector, ExpandsCorrectly) {
    Vector *vector = create_vector(8, sizeof(int));

    for (int i = 0; i < 16; i++) {
        vec_push_back(vector, &i);
    }

    EXPECT_EQ(vector->size, 16);
    EXPECT_EQ(vector->capacity, 16);
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(*(int*)vec_find(vector, i), i);
    }

   free_vector(vector);
}


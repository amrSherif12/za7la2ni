#include <gtest/gtest.h>

extern "C" {
    #include "z_string.h"
}

TEST(String, CreatedCorrectly) {
    String *str = create_str();

    EXPECT_EQ(str->capacity, STRING_INIT_CAP);
    EXPECT_EQ(str->size, 0);
    EXPECT_NE(str->data, nullptr);
}

TEST(String, AddsCharacters) {
    String *str = create_str();
    push_back(str, 'a');

    EXPECT_EQ(str->size, 1);
    EXPECT_EQ(str->data[0], 'a');
}


TEST(String, ExpandsWhenFull) {
    String *str = create_str();

    for (int i = 0; i < STRING_INIT_CAP + 5; i++) {
        push_back(str, 'a');
    }

    EXPECT_EQ(str->capacity, STRING_INIT_CAP * 2);
    EXPECT_EQ(str->size, STRING_INIT_CAP + 5);
    EXPECT_EQ(str->data[STRING_INIT_CAP + 4], 'a');
}
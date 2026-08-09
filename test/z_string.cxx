#include <gtest/gtest.h>

extern "C" {
    #include "z_string.h"
}

TEST(String, AddsCharacters) {
    String *str = create_str();
    str_push_back(str, 'a');

    EXPECT_EQ(str->size, 1);
    EXPECT_EQ(str->data[0], 'a');

    free_str(str);
}


TEST(String, ExpandsWhenFull) {
    String *str = create_str();

    for (int i = 0; i < STRING_INIT_CAP + 5; i++) {
        str_push_back(str, 'a');
    }

    EXPECT_EQ(str->capacity, STRING_INIT_CAP * 2);
    EXPECT_EQ(str->size, STRING_INIT_CAP + 5);
    EXPECT_EQ(str->data[STRING_INIT_CAP + 4], 'a');

    free_str(str);
}
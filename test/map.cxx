#include <gtest/gtest.h>

extern "C" {
    #include "map.h"
    #include "z_string.h"
}

TEST(Map, InsertsFindsData) {
    constexpr size_t data_size = 1000;

    Map *map = create_map(data_size, sizeof(int));
    String *strs[data_size];
    int *nums = (int*)calloc(data_size, sizeof(int));

    for (int i = 0; i < data_size; i++) {
        strs[i] = create_str();
        push_back(strs[i], i + '0');
        nums[i] = i;
        hash(map, strs[i]->data, &nums[i]);
    }

    for (int i = 0; i < data_size; i++) {
        EXPECT_EQ(nums[i], *(int*)find(map, strs[i]->data));
    }

    free(nums);
    for (int i = 0; i < data_size; i++) free_str(strs[i]);
    free(map);
}


TEST(Map, UpdatesData) {
    constexpr size_t data_size = 1000;

    Map *map = create_map(data_size, sizeof(int));
    String *strs[data_size];
    int *nums = (int*)calloc(data_size, sizeof(int));

    for (int i = 0; i < data_size; i++) {
        strs[i] = create_str();
        push_back(strs[i], i + '0');

        nums[i] = i;
        hash(map, strs[i]->data, &nums[i]);

        nums[i] = i + 1;
        hash(map, strs[i]->data, &nums[i]);
    }

    for (int i = 0; i < data_size; i++) {
        EXPECT_EQ(nums[i], *(int*)find(map, strs[i]->data));
    }

    free(nums);
    for (int i = 0; i < data_size; i++) free_str(strs[i]);
    free(map);
}
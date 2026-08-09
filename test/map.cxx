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
        char buf[16];
        int len = snprintf(buf, sizeof(buf), "%d", i);
        for (int j = 0; j < len; j++) {
            str_push_back(strs[i], buf[j]);
        }
        nums[i] = i;
        map_hash(map, strs[i]->data, strs[i]->size, &nums[i]);
    }

    for (int i = 0; i < data_size; i++) {
        EXPECT_EQ(nums[i], *(int*)map_find(map, strs[i]->data, strs[i]->size));
    }

    free(nums);
    for (int i = 0; i < data_size; i++) free_str(strs[i]);
    free_map(map);
}


TEST(Map, UpdatesData) {
    constexpr size_t data_size = 1000;

    Map *map = create_map(data_size, sizeof(int));
    String *strs[data_size];
    int *nums = (int*)calloc(data_size, sizeof(int));

    for (int i = 0; i < data_size; i++) {
        strs[i] = create_str();
        char buf[16];
        int len = snprintf(buf, sizeof(buf), "%d", i);
        for (int j = 0; j < len; j++) {
            str_push_back(strs[i], buf[j]);
        }
        nums[i] = i;
        map_hash(map, strs[i]->data, strs[i]->size, &nums[i]);

        nums[i] = i + 1;
        map_hash(map, strs[i]->data, strs[i]->size, &nums[i]);
    }

    for (int i = 0; i < data_size; i++) {
        EXPECT_EQ(nums[i], *(int*)map_find(map, strs[i]->data, strs[i]->size));
    }

    free(nums);
    for (int i = 0; i < data_size; i++) free_str(strs[i]);
    free_map(map);
}
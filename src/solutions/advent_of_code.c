/*****************************************************
Create Date:        2024-11-12
Author:             Oskar Bahner Hansen
Email:              cph-oh82@cphbusiness.dk
Description:        exercise template
License:            none
*****************************************************/

#include "../../include/obh/incl.h"
#include "../../include/obh/c_log.h"
#include "../../include/obh/util.h"
#include "../../include/stb/stb_ds.h"

typedef struct int_array_t {
    union {
        int key;
        int value;
    };
} int_array_t;

typedef struct int_map_t {
    int key;
    int value;
} int_map_t;

int int_array_cmp(const void* a, const void* b)
{
    return ((int_array_t*)a)->key - ((int_array_t*)b)->key;
}

int aoc_1_a()
{
    int res = 0, idx = 0;
    int_array_t *list_l_r[2] = { 0 };

    sds lists = sdsfread(sdsempty(), "resources/aoc_1_data.txt");

    for (char *lists_ptr = lists; *lists_ptr != 0; lists_ptr++) {
        char *end_ptr;
        char c = *lists_ptr;
        if (isdigit(c)) {
            int_array_t val = {strtoll(lists_ptr, &end_ptr, 10)};
            arrput(list_l_r[idx], val);
            idx = (idx + 1) % 2;
            lists_ptr = end_ptr - 1;
        }
    }

    sdsfree(lists);

    qsort(list_l_r[0], arrlen(list_l_r[0]), sizeof(int_array_t), int_array_cmp);
    qsort(list_l_r[1], arrlen(list_l_r[1]), sizeof(int_array_t), int_array_cmp);

    for (int i = 0; i < arrlen(list_l_r[0]); ++i) {
        res += abs(list_l_r[0][i].key - list_l_r[1][i].key);
        //printf("d(%d %d) = %d\n", list_l_r[0][i].key, list_l_r[1][i].key, abs(list_l_r[0][i].key - list_l_r[1][i].key));
    }

    return res;
}

int aoc_1_b()
{
    int res = 0, idx = 0;
    int_map_t *map_l_r[2] = { 0 };

    sds lists = sdsfread(sdsempty(), "resources/aoc_1_data.txt");

    for (char *lists_ptr = lists; *lists_ptr != 0; lists_ptr++) {
        char *end_ptr;
        char c = *lists_ptr;
        if (isdigit(c)) {
            int val = {strtoll(lists_ptr, &end_ptr, 10)};

            if (hmgeti(map_l_r[idx], val) < 0) {
                hmput(map_l_r[idx], val, 1);
            } else {
                hmput(map_l_r[idx], val, 1 + hmget(map_l_r[idx], val));
            }
            idx = (idx + 1) % 2;
            lists_ptr = end_ptr - 1;
        }
    }

    sdsfree(lists);

    for (int i = 0; i < hmlen(map_l_r[0]); ++i) {
        if (hmgeti(map_l_r[1], map_l_r[0][i].key) < 0)
            continue;
        res += map_l_r[0][i].key * hmget(map_l_r[1], map_l_r[0][i].key);
        //res += abs(map_l_r[0][i].key - map_l_r[1][i].key);
        //printf("%d occurs %d times in right, %d times in left\n",
                //map_l_r[0][i].key, map_l_r[0][i].value, hmget(map_l_r[1], map_l_r[0][i].key));
    }

    return res;
}

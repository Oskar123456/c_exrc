/*****************************************************
Create Date:        2024-12-01
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

int aoc_2_a()
{
    sds reports_str = sdsfread(sdsempty(), "resources/aoc_2_data.txt");

    int report_idx = 0;
    i32 **reports = NULL;
    arrput(reports, NULL);

    for (char *reports_ptr = reports_str; *reports_ptr != 0; reports_ptr++) {
        char c = *reports_ptr;
        if (isdigit(c)) {
            i32 val = strtoll(reports_ptr, &reports_ptr, 10);
            arrput(reports[report_idx], val);
            reports_ptr--;
        }
        if (c == '\n') {
            report_idx++;
            arrput(reports, NULL);
        }
    }

    int res = 0;

    for (int i = 0; i < arrlen(reports); ++i) {
        if (arrlen(reports[i]) < 1) {
            continue;
        }
        if (arrlen(reports[i]) < 2) {
            res++;
            continue;
        }

        bool is_safe = true;
        int diff_expected = 0;
        for (int j = 1; j < arrlen(reports[i]); ++j) {
            int diff_actual = reports[i][j] - reports[i][j - 1];
            if (diff_actual == 0 || abs(diff_actual) > 3) {
                is_safe = false;
                break;
            } else if (diff_expected == 0) {
                diff_expected = diff_actual;
            } else if ((diff_expected < 0) != (diff_actual < 0)) {
                is_safe = false;
                break;
            }
        }

        //arr_i_print(reports[i], arrlen(reports[i]));
        //printf(" : %s\n", is_safe ? "TRUE" : "FALSE");

        res += is_safe;
    }

    //c_log_info(LOG_TAG, "%d/%d found safe", res, arrlen(reports));

    return res;
}

bool aoc_2_b_verf(int* array)
{
    if (arrlen(array) < 1) {
        return false;
    }
    if (arrlen(array) < 2) {
        return true;
    }

    int diff_expected = 0;
    for (int j = 1; j < arrlen(array); ++j) {
        int diff_actual = array[j] - array[j - 1];
        if (diff_actual == 0 || abs(diff_actual) > 3) {
            return false;
        } else if (diff_expected == 0) {
            diff_expected = diff_actual;
        } else if ((diff_expected < 0) != (diff_actual < 0)) {
            return false;
        }
    }

    return true;
}

int aoc_2_b()
{
    sds reports_str = sdsfread(sdsempty(), "resources/aoc_2_data.txt");

    int report_idx = 0;
    i32 **reports = NULL;
    arrput(reports, NULL);

    for (char *reports_ptr = reports_str; *reports_ptr != 0; reports_ptr++) {
        char c = *reports_ptr;
        if (isdigit(c)) {
            i32 val = strtoll(reports_ptr, &reports_ptr, 10);
            arrput(reports[report_idx], val);
            reports_ptr--;
        }
        if (c == '\n') {
            report_idx++;
            if (reports_ptr - reports_str < sdslen(reports_str) - 1)
                arrput(reports, NULL);
        }
    }

    int res = 0;
    int *test_report = NULL;
    for (int i = 0; i < arrlen(reports); ++i) {
        int fault_idx;
        bool verf = aoc_2_b_verf(reports[i]);

        if (verf) {
            res++;
            continue;
        }

        for (int j = 0; j < arrlen(reports[i]); j++) {
            arrsetlen(test_report, 0);
            for (int k = 0; k < arrlen(reports[i]); ++k) {
                if (k != j)
                    arrput(test_report, reports[i][k]);
            }
            if ((verf = aoc_2_b_verf(test_report)))
                break;
        }

        res += verf;
    }
    arrfree(test_report);

    return res;
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
            int_array_t val = { .key = strtoll(lists_ptr, &end_ptr, 10)};
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
            int val = strtoll(lists_ptr, &end_ptr, 10);

            if (hmgeti(map_l_r[idx], val) < 0) {
                hmput(map_l_r[idx], val, 1);
            } else {
                int c = hmget(map_l_r[idx], val);
                hmput(map_l_r[idx], val, 1 + c);
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

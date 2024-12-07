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

#include <gmp.h>

#include "aoc_ds.c"

bool aoc_7_b_2_help(u64 answer, u64 current, u64 *terms, int idx)
{
    if (idx >= arrlen(terms)) {
        if (current == answer)
            return true;
        return false;
    }

    bool retval = aoc_7_b_2_help(answer, current + terms[idx], terms, idx + 1);
    if (!retval)
        retval = aoc_7_b_2_help(answer, current * terms[idx], terms, idx + 1);
    if (!retval) {
        int next_len = (int)log10(terms[idx]) + 1;
        u64 cat = current * pow(10, next_len) + terms[idx];
        retval = aoc_7_b_2_help(answer, cat, terms, idx + 1);
    }

    return retval;
}

int aoc_7_b_2()
{
    u64 result = 0;

    u64 **eqs = NULL;

    FILE *f = fopen("resources/aoc_data/aoc_7_data.txt", "r");
    char line[4000];
    while ((fgets(line, 4000, f)) != NULL) {
        arrput(eqs, NULL);
        char *line_ptr = line;
        while (*line_ptr && *line_ptr != '\n') {
            if (isdigit(*line_ptr)) {
                i64 ll = strtoll(line_ptr, &line_ptr, 10);
                assert(ll != LONG_MAX);
                assert(ll != LONG_MIN);
                arrput(eqs[arrlen(eqs) - 1], ll);
            }
            else
                line_ptr++;
        }
    }
    fclose(f);

    for (int i = 0; i < arrlen(eqs); ++i) {
        if (aoc_7_b_2_help(eqs[i][0], eqs[i][1], eqs[i], 2))
            result += eqs[i][0];
        arrfree(eqs[i]);
    }
    printf("aoc_7_b_2: %lu\n", result);

    arrfree(eqs);

    return result;
}

bool aoc_7_a_2_help(u64 answer, u64 current, u64 *terms, int idx)
{
    if (idx >= arrlen(terms)) {
        if (current == answer)
            return true;
        return false;
    }

    bool retval = aoc_7_b_2_help(answer, current + terms[idx], terms, idx + 1);
    if (!retval)
        retval = aoc_7_b_2_help(answer, current * terms[idx], terms, idx + 1);

    return retval;
}

int aoc_7_a_2()
{
    u64 result = 0;

    u64 **eqs = NULL;

    FILE *f = fopen("resources/aoc_data/aoc_7_data.txt", "r");
    //FILE *f = fopen("aoc7testdata.txt", "r");
    char line[4000];
    while ((fgets(line, 4000, f)) != NULL) {
        arrput(eqs, NULL);
        char *line_ptr = line;
        while (*line_ptr && *line_ptr != '\n') {
            if (isdigit(*line_ptr)) {
                i64 ll = strtoll(line_ptr, &line_ptr, 10);
                assert(ll != LONG_MAX);
                assert(ll != LONG_MIN);
                arrput(eqs[arrlen(eqs) - 1], ll);
            }
            else
                line_ptr++;
        }
    }
    fclose(f);

    for (int i = 0; i < arrlen(eqs); ++i) {
        if (aoc_7_a_2_help(eqs[i][0], eqs[i][1], eqs[i], 2))
            result += eqs[i][0];
        arrfree(eqs[i]);
    }
    printf("aoc_7_b_2: %lu\n", result);

    arrfree(eqs);

    return result;
}

sds bigint_from_string(const char *ptr, const char **end_ptr)
{
    const char *ptr_ptr = ptr;
    while (*ptr_ptr && *ptr_ptr != '-' && !isdigit(*ptr_ptr))
        ptr_ptr++;
    if (!*ptr_ptr)
        return NULL;
    bool is_neg = *ptr_ptr == '-';
    if (is_neg) ptr_ptr++;
    if (!isdigit(*ptr_ptr))
        return NULL;

    int len = 0;
    while (isdigit(*ptr_ptr)) {
        len++;
        ptr_ptr++;
    }
    sds bi = sdscatlen(sdsempty(), ptr_ptr - len, len);

    if (end_ptr && *end_ptr)
        *end_ptr = ptr_ptr;

    return bi;
}

bool aoc_7_b_help(mpz_t result, mpz_t actual, mpz_t *vals, int len, int idx, char *ops)
{
    if (idx >= len) {
        if (mpz_cmp(result, actual) == 0)
            return true;
        else return false;
    }

    bool retval = false;

    mpz_t tmp_add; mpz_init(tmp_add);
    mpz_t tmp_mul; mpz_init(tmp_mul);
    mpz_t tmp_cat; mpz_init(tmp_cat);

    mpz_add(tmp_add, actual, vals[idx]);
    mpz_mul(tmp_mul, actual, vals[idx]);

    char a[400];
    mpz_get_str(a, 10, actual);
    mpz_get_str(&a[strlen(a)], 10, vals[idx]);
    mpz_set_str(tmp_cat, a, 10);

    ops[idx] = '+';
    if (aoc_7_b_help(result, tmp_add, vals, len, idx + 1, ops))
        retval = true;
    if (!retval) {
        ops[idx] = '*';
        if (aoc_7_b_help(result, tmp_mul, vals, len, idx + 1, ops))
            retval = true;
    }
    if (!retval) {
        ops[idx] = '|';
        if (aoc_7_b_help(result, tmp_cat, vals, len, idx + 1, ops))
            retval = true;
    }

    mpz_clear(tmp_add);
    mpz_clear(tmp_mul);
    mpz_clear(tmp_cat);

    return retval;
}

int aoc_7_b()
{
    i64 result = 0;

    sds *results = NULL;
    sds **terms = NULL;

    FILE *f = fopen("resources/aoc_data/aoc_7_data.txt", "r");
    //FILE *f = fopen("aoc7testdata.txt", "r");
    char line[4000];
    while ((fgets(line, 4000, f)) != NULL) {
        const char *line_ptr = line;
        sds bi = bigint_from_string(line_ptr, &line_ptr);
        arrput(results, bi);
        arrput(terms, NULL);
        while (*line_ptr && *line_ptr != '\n') {
            sds bi = bigint_from_string(line_ptr, &line_ptr);
            if (bi == NULL)
                break;
            arrput(terms[arrlen(terms) - 1], bi);
        }
    }
    fclose(f);

    mpz_t results_mpz[arrlen(results)];
    mpz_t *terms_mpz[arrlen(results)];
    mpz_t final;
    mpz_init(final);
    mpz_set_ui(final, 0);

    for (int i = 0; i < arrlen(results); ++i) {
        mpz_init(results_mpz[i]);
        mpz_set_str(results_mpz[i], results[i], 10);
        terms_mpz[i] = calloc(arrlen(terms[i]), sizeof(mpz_t));
        for (int j = 0; j < arrlen(terms[i]); ++j) {
            mpz_init(terms_mpz[i][j]);
            mpz_set_str(terms_mpz[i][j], terms[i][j], 10);
        }
    }

    for (int i = 0; i < arrlen(results); ++i) {
        char ops[512];
        if (aoc_7_b_help(results_mpz[i], terms_mpz[i][0], terms_mpz[i], arrlen(terms[i]), 1, ops)) {
            mpz_add(final, final, results_mpz[i]);
            //printf("[SUCCESS] %.*s: ", (int)sdslen(results[i]), results[i]);
            //for (int j = 0; j < arrlen(terms[i]); ++j) {
                //printf("%s", terms[i][j]);
                //if (j < arrlen(terms[i]) - 1)
                    //printf(" %c ", ops[j + 1]);
            //}
            //printf("\n");
        } else {
            //printf("[FAIL] %.*s:", (int)sdslen(results[i]), results[i]);
            //for (int j = 0; j < arrlen(terms[i]); ++j) {
                //printf(" %s", terms[i][j]);
            //}
            //printf("\n");
        }
    }

    char str[512];
    printf("\n[aoc_7_b] FINAL ANSWER: %s\n\n", mpz_get_str(str, 10, final));

    for (int i = 0; i < arrlen(results); ++i) {
        mpz_clear(results_mpz[i]);
        for (int j = 0; j < arrlen(terms[i]); ++j) {
            sdsfree(terms[i][j]);
            mpz_clear(terms_mpz[i][j]);
        }
        sdsfree(results[i]);
        free(terms_mpz[i]);
        arrfree(terms[i]);
    }
    arrfree(results);
    arrfree(terms);
    mpz_clear(final);

    return result;
}

bool aoc_7_a_help(mpz_t result, mpz_t actual, mpz_t *vals, int len, int idx, char *ops)
{
    if (idx >= len) {
        if (mpz_cmp(result, actual) == 0)
            return true;
        else return false;
    }

    bool retval = false;

    mpz_t tmp_add; mpz_init(tmp_add);
    mpz_t tmp_mul; mpz_init(tmp_mul);

    mpz_add(tmp_add, actual, vals[idx]);
    mpz_mul(tmp_mul, actual, vals[idx]);

    ops[idx] = '+';
    if (aoc_7_a_help(result, tmp_add, vals, len, idx + 1, ops))
        retval = true;
    if (!retval) {
        ops[idx] = '*';
        if (aoc_7_a_help(result, tmp_mul, vals, len, idx + 1, ops))
            retval = true;
    }

    mpz_clear(tmp_add);
    mpz_clear(tmp_mul);

    return retval;
}

int aoc_7_a()
{
    i64 result = 0;

    sds *results = NULL;
    sds **terms = NULL;

    FILE *f = fopen("resources/aoc_data/aoc_7_data.txt", "r");
    //FILE *f = fopen("aoc7testdata.txt", "r");
    char line[4000];
    while ((fgets(line, 4000, f)) != NULL) {
        const char *line_ptr = line;
        sds bi = bigint_from_string(line_ptr, &line_ptr);
        arrput(results, bi);
        arrput(terms, NULL);
        while (*line_ptr && *line_ptr != '\n') {
            sds bi = bigint_from_string(line_ptr, &line_ptr);
            if (bi == NULL)
                break;
            arrput(terms[arrlen(terms) - 1], bi);
        }
    }
    fclose(f);

    mpz_t results_mpz[arrlen(results)];
    mpz_t *terms_mpz[arrlen(results)];
    mpz_t final;
    mpz_init(final);
    mpz_set_ui(final, 0);

    for (int i = 0; i < arrlen(results); ++i) {
        mpz_init(results_mpz[i]);
        mpz_set_str(results_mpz[i], results[i], 10);
        terms_mpz[i] = calloc(arrlen(terms[i]), sizeof(mpz_t));
        for (int j = 0; j < arrlen(terms[i]); ++j) {
            mpz_init(terms_mpz[i][j]);
            mpz_set_str(terms_mpz[i][j], terms[i][j], 10);
        }
    }

    for (int i = 0; i < arrlen(results); ++i) {
        char ops[512];
        if (aoc_7_a_help(results_mpz[i], terms_mpz[i][0], terms_mpz[i], arrlen(terms[i]), 1, ops)) {
            mpz_add(final, final, results_mpz[i]);
            //printf("[SUCCESS] %.*s: ", (int)sdslen(results[i]), results[i]);
            //for (int j = 0; j < arrlen(terms[i]); ++j) {
                //printf("%s", terms[i][j]);
                //if (j < arrlen(terms[i]) - 1)
                    //printf(" %c ", ops[j + 1]);
            //}
            //printf("\n");
        } else {
            //printf("[FAIL] %.*s:", (int)sdslen(results[i]), results[i]);
            //for (int j = 0; j < arrlen(terms[i]); ++j) {
                //printf(" %s", terms[i][j]);
            //}
            //printf("\n");
        }
    }

    char str[512];
    printf("\n[aoc_7_a] FINAL ANSWER: %s\n\n", mpz_get_str(str, 10, final));

    for (int i = 0; i < arrlen(results); ++i) {
        mpz_clear(results_mpz[i]);
        for (int j = 0; j < arrlen(terms[i]); ++j) {
            sdsfree(terms[i][j]);
            mpz_clear(terms_mpz[i][j]);
        }
        sdsfree(results[i]);
        free(terms_mpz[i]);
        arrfree(terms[i]);
    }
    arrfree(results);
    arrfree(terms);
    mpz_clear(final);

    return result;
}

int aoc_6_b()
{
    i64 result = 0;

    i8 dir;
    char *dir_chars = "^>v<";
    ivec2_t dirs[] = { {  0, -1 }, {  1,  0 }, {  0,  1 }, { -1,  0 } };
    ivec2_t pos;

    sds *data = NULL;
    FILE *f = fopen("resources/aoc_data/aoc_6_data.txt", "r");
    //FILE *f = fopen("aoc6testdata.txt", "r");
    char line[4000];
    while ((fgets(line, 4000, f)) != NULL) {
        sds l = sdscatlen(sdsempty(), line, strlen(line) - 1);
        char *gp;
        if ((gp = strpbrk(l, dir_chars))) {
            pos.x = gp - l;
            pos.y = arrlen(data);
            dir = strchr(dir_chars, *gp) - dir_chars;
            //printf("%d, %d dir: %d\n", pos.y, pos.x, dir);
        }
        arrput(data, l);
    }
    fclose(f);

    bool visited_dirs[arrlen(data)][sdslen(data[0]) * 4];
    for (int i = 0; i < arrlen(data); ++i) {
        for (int j = 0; j < sdslen(data[i]); ++j) {
            if (data[i][j] != '.')
                continue;

            memset(visited_dirs, 0, sizeof(bool) * arrlen(data) * sdslen(data[i]) * 4);

            data[i][j] = '#';

            ivec2_t npos = pos;
            i8 ndir = dir;
            int c = 0;
            for (;;) {
                c++;

                visited_dirs[npos.y][npos.x * 4 + ndir] = 1;

                npos.x += dirs[ndir].x;
                npos.y += dirs[ndir].y;

                if ( npos.y < 0 || npos.y >= arrlen(data) || npos.x < 0 || npos.x >= sdslen(data[npos.y])) {
                    break;
                }

                if (visited_dirs[npos.y][npos.x * 4 + ndir]) {
                    result++;
                    break;
                }

                if (data[npos.y][npos.x] == '#') {
                    npos.x -= dirs[ndir].x;
                    npos.y -= dirs[ndir].y;
                    ndir = (ndir + 1) % 4;
                    continue;
                }
            }

            data[i][j] = '.';
        }
    }

    for (int i = 0; i < arrlen(data); ++i)
        sdsfree(data[i]);
    arrfree(data);

    return result;
}

int aoc_6_a()
{
    i64 result = 0;
    sds *data = NULL;

    i8 dir;
    char *dir_chars = "^>v<";
    ivec2_t dirs[] = { {  0, -1 }, {  1,  0 }, {  0,  1 }, { -1,  0 } };
    ivec2_t pos;

    FILE *f = fopen("resources/aoc_data/aoc_6_data.txt", "r");
    //FILE *f = fopen("aoc6testdata.txt", "r");
    char line[4000];
    while ((fgets(line, 4000, f)) != NULL) {
        sds l = sdscatlen(sdsempty(), line, strlen(line) - 1);
        char *gp;
        if ((gp = strpbrk(l, "<>^v"))) {
            pos.x = gp - l;
            pos.y = arrlen(data);
            if (*gp == '^')
                dir = 0;
            if (*gp == '>')
                dir = 1;
            if (*gp == 'v')
                dir = 2;
            if (*gp == '<')
                dir = 3;
        }
        arrput(data, l);
    }

    for (;;) {
        data[pos.y][pos.x] = 'X';

        pos.x += dirs[dir].x;
        pos.y += dirs[dir].y;

        if ( pos.y < 0 || pos.y >= arrlen(data)
                || pos.x < 0 || pos.x >= sdslen(data[pos.y])) {

            break;
        }

        if (data[pos.y][pos.x] == '#') {
            pos.x -= dirs[dir].x;
            pos.y -= dirs[dir].y;
            data[pos.y][pos.x] = dir_chars[dir];
            dir = (dir + 1) % 4;
            continue;
        }

        data[pos.y][pos.x] = dir_chars[dir];
    }

    for (int i = 0; i < arrlen(data); ++i) {
        for (int j = 0; j < sdslen(data[i]); ++j) {
            result += data[i][j] == 'X';
        }
    }

    for (int i = 0; i < arrlen(data); ++i)
        sdsfree(data[i]);
    arrfree(data);
    fclose(f);

    return result;
}

static ivec2_t *pairs;
int aoc_5_a_cmp(const void* a, const void* b)
{
    int ai = *(int*)a; int bi = *(int*)b;

    for (int i = 0; i < arrlen(pairs); ++i) {
        if (pairs[i].x == ai && pairs[i].y == bi)
            return -1;
        if (pairs[i].y == ai && pairs[i].x == bi)
            return 1;
    }

    return 0;
}

int aoc_5_b()
{
    i64 result = 0;
    sds data = sdsfread(sdsempty(), "resources/aoc_data/aoc_5_data.txt");
    char *data_ptr = data;
    int **updates = NULL, updates_idx = -1;
    int **updates_sorted = NULL, updates_sorted_idx = -1;

    for (;;) {
        if (*data_ptr == '\n' && *(data_ptr + 1) == '\n')
            break;
        while (!isdigit(*data_ptr)) data_ptr++;
        i64 a = strtoll(data_ptr, &data_ptr, 10);
        while (!isdigit(*data_ptr)) data_ptr++;
        i64 b = strtoll(data_ptr, &data_ptr, 10);
        ivec2_t p = { a, b };
        arrput(pairs, p);
    }

    for (;;) {
        if (data_ptr - data >= sdslen(data) - 1)
            break;
        if (*data_ptr == '\n') {
            arrput(updates, NULL);
            updates_idx++;
        }
        while (!isdigit(*data_ptr)) data_ptr++;
        i64 a = strtoll(data_ptr, &data_ptr, 10);
        arrput(updates[updates_idx], a);
    }

    for (int i = 0; i < arrlen(updates); ++i) {
        arrput(updates_sorted, NULL);
        updates_sorted_idx++;
        arrsetlen(updates_sorted[updates_sorted_idx], arrlen(updates[i]));
        memcpy(updates_sorted[updates_sorted_idx], updates[i], sizeof(int) * arrlen(updates[i]));
        qsort(updates_sorted[updates_sorted_idx], arrlen(updates[i]), sizeof(int), aoc_5_a_cmp);
    }

    for (int i = 0; i < arrlen(updates); ++i) {
        if (memcmp(updates[i], updates_sorted[i], arrlen(updates[i]) * sizeof(int)) == 0)
            continue;
        result += updates_sorted[i][arrlen(updates_sorted[i]) / 2];
    }

    for (int i = 0; i < arrlen(updates); ++i)
        arrfree(updates[i]);
    for (int i = 0; i < arrlen(updates_sorted); ++i)
        arrfree(updates_sorted[i]);
    arrfree(pairs);
    arrfree(updates_sorted);
    arrfree(updates);
    sdsfree(data);

    return result;
}

int aoc_5_a()
{
    i64 result = 0;
    sds data = sdsfread(sdsempty(), "resources/aoc_data/aoc_5_data.txt");
    char *data_ptr = data;
    int **updates = NULL, updates_idx = -1;
    int **updates_sorted = NULL, updates_sorted_idx = -1;

    for (;;) {
        if (*data_ptr == '\n' && *(data_ptr + 1) == '\n')
            break;
        while (!isdigit(*data_ptr)) data_ptr++;
        i64 a = strtoll(data_ptr, &data_ptr, 10);
        while (!isdigit(*data_ptr)) data_ptr++;
        i64 b = strtoll(data_ptr, &data_ptr, 10);
        ivec2_t p = { a, b };
        arrput(pairs, p);
    }

    for (;;) {
        if (data_ptr - data >= sdslen(data) - 1)
            break;
        if (*data_ptr == '\n') {
            arrput(updates, NULL);
            updates_idx++;
        }
        while (!isdigit(*data_ptr)) data_ptr++;
        i64 a = strtoll(data_ptr, &data_ptr, 10);
        arrput(updates[updates_idx], a);
    }

    for (int i = 0; i < arrlen(updates); ++i) {
        arrput(updates_sorted, NULL);
        updates_sorted_idx++;
        arrsetlen(updates_sorted[updates_sorted_idx], arrlen(updates[i]));
        memcpy(updates_sorted[updates_sorted_idx], updates[i], sizeof(int) * arrlen(updates[i]));
        qsort(updates_sorted[updates_sorted_idx], arrlen(updates[i]), sizeof(int), aoc_5_a_cmp);
    }

    for (int i = 0; i < arrlen(updates); ++i) {
        if (memcmp(updates[i], updates_sorted[i], arrlen(updates[i]) * sizeof(int)) == 0)
            result += updates[i][arrlen(updates[i]) / 2];
    }

    for (int i = 0; i < arrlen(updates); ++i)
        arrfree(updates[i]);
    for (int i = 0; i < arrlen(updates_sorted); ++i)
        arrfree(updates_sorted[i]);
    arrfree(pairs);
    arrfree(updates_sorted);
    arrfree(updates);
    sdsfree(data);

    return result;
}

int aoc_4_b()
{
    i64 result = 0, dm_idx = -1;
    sds data_str = sdsfread(sdsempty(), "resources/aoc_data/aoc_4_data.txt");
    sds *dm = NULL;

    for (int i = 0; i < sdslen(data_str); ++i) {
        arrput(dm, sdsempty());
        dm_idx++;

        int j;
        for (j = i; data_str[j] != '\n' && data_str[j]; ++j);
        dm[dm_idx] = sdscatlen(dm[dm_idx], &data_str[i], j - i);

        i = j;
    }

    for (int i = 0; i < arrlen(dm); ++i) {
        for (int j = 0; j < sdslen(dm[i]); ++j) {
            if (dm[i][j] != 'A')
                continue;
            if (i - 1 < 0 || j - 1 < 0
                    || i + 1 >= arrlen(dm)
                    || j + 1 >= sdslen(dm[i]))
                continue;
            char nw = dm[i - 1][j - 1];
            char ne = dm[i - 1][j + 1];
            char se = dm[i + 1][j + 1];
            char sw = dm[i + 1][j - 1];
            if (((nw == 'M' || nw == 'S') && (se == 'M' || se == 'S') && nw != se)
                    && ((ne == 'M' || ne == 'S') && (sw == 'M' || sw == 'S') && ne != sw))
                result++;
        }
    }

    for (int i = 0; i < arrlen(dm); ++i)
        sdsfree(dm[i]);
    arrfree(dm);
    sdsfree(data_str);
    return result;
}

int aoc_4_a()
{
    i64 result = 0, dm_idx = -1;
    sds data_str = sdsfread(sdsempty(), "resources/aoc_data/aoc_4_data.txt");
    sds *dm = NULL;

    for (int i = 0; i < sdslen(data_str); ++i) {
        arrput(dm, sdsempty());
        dm_idx++;

        int j;
        for (j = i; data_str[j] != '\n' && data_str[j]; ++j);
        dm[dm_idx] = sdscatlen(dm[dm_idx], &data_str[i], j - i);

        i = j;
    }

    for (int i = 0; i < arrlen(dm); ++i) {
        for (int j = 0; j < sdslen(dm[i]); ++j) {
            if (dm[i][j] != 'X')
                continue;
            char *xmas = "XMAS";
            bool n, ne, e, se, s, sw, w, nw;
            n = ne = e = se = s = sw = w = nw = true;
            for (int k = 1; k < strlen(xmas); ++k) {
                if (!(i - k >= 0 && dm[i - k][j] == xmas[k]))
                    n = false;
                if (!(i - k >= 0 && j + k < sdslen(dm[i]) && dm[i - k][j + k] == xmas[k]))
                    ne = false;
                if (!(j + k < sdslen(dm[i]) && dm[i][j + k] == xmas[k]))
                    e = false;
                if (!(i + k < arrlen(dm) && j + k < sdslen(dm[i]) && dm[i + k][j + k] == xmas[k]))
                    se = false;
                if (!(i + k < arrlen(dm) && dm[i + k][j] == xmas[k]))
                    s = false;
                if (!(i + k < arrlen(dm) && j - k >= 0 && dm[i + k][j - k] == xmas[k]))
                    sw = false;
                if (!(j - k >= 0 && dm[i][j - k] == xmas[k]))
                    w = false;
                if (!(i - k >= 0 && j - k >= 0 && dm[i - k][j - k] == xmas[k]))
                    nw = false;
            }
            result += n + ne + e + se + s + sw + w + nw;
        }
    }

    for (int i = 0; i < arrlen(dm); ++i)
        sdsfree(dm[i]);
    arrfree(dm);
    sdsfree(data_str);
    return result;
}

int aoc_3_b()
{
    i64 result = 0;

    sds data_str = sdsfread(sdsempty(), "resources/aoc_data/aoc_3_data.txt");

    int err_code; u64 err_offset;
    PCRE2_SPTR pattern = (PCRE2_SPTR) "mul\\([0-9]+,[0-9]+\\)";
    pcre2_code *regex = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED,
            0, &err_code, &err_offset, NULL);
    pcre2_match_data *match_data;

    for (int data_str_off = 0; data_str_off < sdslen(data_str);) {
        match_data = pcre2_match_data_create_from_pattern(regex, NULL);
        int match_num = pcre2_match(regex, (PCRE2_SPTR)data_str, PCRE2_ZERO_TERMINATED,
                data_str_off, 0, match_data, NULL);

        if (match_num < 1)
            break;

        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);

        char *match_ptr = &data_str[ovector[0]];

        bool enable = true;
        for (char *matchptr_b = match_ptr - 1; matchptr_b >= data_str; matchptr_b--) {
            if (*matchptr_b == 'd') {
                if (strncmp(matchptr_b, "do()", strlen("do()")) == 0) {
                    break;
                }
                if (strncmp(matchptr_b, "don't()", strlen("don't()")) == 0) {
                    enable = false;
                    break;
                }
            }
        }

        while (!isdigit(*match_ptr))
            match_ptr++;
        long a = strtol(match_ptr, &match_ptr, 10);
        while (!isdigit(*match_ptr))
            match_ptr++;
        long b = strtol(match_ptr, &match_ptr, 10);
        if (enable)
            result += a * b;

        //printf("%.*s: %ld * %ld = %ld (total: %ld\n",
        //        match_len, match_ptr_forp, a, b, a * b, result);
        pcre2_match_data_free(match_data);
        data_str_off = ovector[1];
    }

    pcre2_match_data_free(match_data);
    pcre2_code_free(regex);
    sdsfree(data_str);
    return result;
}

int aoc_3_a()
{
    i64 result = 0;

    sds data_str = sdsfread(sdsempty(), "resources/aoc_data/aoc_3_data.txt");

    int err_code; u64 err_offset;
    PCRE2_SPTR pattern = (PCRE2_SPTR) "mul\\([0-9]+,[0-9]+\\)";
    pcre2_code *regex = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED,
            0, &err_code, &err_offset, NULL);
    pcre2_match_data *match_data;

    for (int data_str_off = 0; data_str_off < sdslen(data_str);) {
        match_data = pcre2_match_data_create_from_pattern(regex, NULL);
        int match_num = pcre2_match(regex, (PCRE2_SPTR)data_str, PCRE2_ZERO_TERMINATED,
                data_str_off, 0, match_data, NULL);

        if (match_num < 1)
            break;

        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);

        char *match_ptr = &data_str[ovector[0]];

        while (!isdigit(*match_ptr))
            match_ptr++;
        long a = strtol(match_ptr, &match_ptr, 10);
        while (!isdigit(*match_ptr))
            match_ptr++;
        long b = strtol(match_ptr, &match_ptr, 10);
        result += a * b;

        //printf("%.*s: %ld * %ld = %ld (total: %ld\n",
        //        match_len, match_ptr_forp, a, b, a * b, result);
        pcre2_match_data_free(match_data);
        data_str_off = ovector[1];
    }

    pcre2_match_data_free(match_data);
    pcre2_code_free(regex);
    sdsfree(data_str);
    return result;
}

int aoc_2_a()
{
    sds reports_str = sdsfread(sdsempty(), "resources/aoc_data/aoc_2_data.txt");

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

    for (int i = 0; i < arrlen(reports); ++i) {
        arrfree(reports[i]);
    }
    arrfree(reports);
    sdsfree(reports_str);
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
    sds reports_str = sdsfread(sdsempty(), "resources/aoc_data/aoc_2_data.txt");

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

    for (int i = 0; i < arrlen(reports); ++i) {
        arrfree(reports[i]);
    }
    arrfree(reports);
    arrfree(test_report);
    sdsfree(reports_str);

    return res;
}

int aoc_1_a()
{
    int res = 0, idx = 0;
    int_array_t *list_l_r[2] = { 0 };

    sds lists = sdsfread(sdsempty(), "resources/aoc_data/aoc_1_data.txt");

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

    arrfree(list_l_r[0]);
    arrfree(list_l_r[1]);
    return res;
}

int aoc_1_b()
{
    int res = 0, idx = 0;
    int_map_t *map_l_r[2] = { 0 };

    sds lists = sdsfread(sdsempty(), "resources/aoc_data/aoc_1_data.txt");

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

    hmfree(map_l_r[0]);
    hmfree(map_l_r[1]);

    return res;
}

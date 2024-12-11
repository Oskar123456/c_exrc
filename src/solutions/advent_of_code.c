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

#define MAXLEN 512

int strnumlen(const char *str)
{
    const char *str_ptr = str;
    while (*str_ptr && isdigit(*str_ptr)) str_ptr++;
    return str_ptr - str;
}

typedef struct str_int_map_t {
    sds key;
    u64 value;
} str_int_map_t;

static str_int_map_t *aoc_11_cache;

void aoc11cacheput(sds key, u64 value)
{
    str_int_map_t kv = { .key = key, .value = value };
    arrput(aoc_11_cache, kv);
}

i64 aoc11cachefind(sds key)
{
    for (int i = 0; i < arrlen(aoc_11_cache); ++i) {
        if (strcmp(key, aoc_11_cache[i].key) == 0) {
            printf("found %s: val: %lu\n", key, aoc_11_cache[i].value);
            return aoc_11_cache[i].value;
        }
    }
    return -1;
}

u64 aoc_11_b_rec(sds stone, int blink)
{
    sds stone_w_blink = sdscat(sdsempty(), stone);
    stone_w_blink = sdscatprintf(stone_w_blink, " blink: %d", blink);

    u64 res;

    if (blink == 0) {
        //printf("base case: \"%s\"\n", stone);
        shput(aoc_11_cache, stone_w_blink, 1);
        //aoc11cacheput(stone_w_blink, 1);
    } else if (shgeti(aoc_11_cache, stone_w_blink) < 0) {
        if (strncmp(stone, "0", 1) == 0) {
            sds next = sdscatprintf(sdsempty(), "%d", 1);
            res = aoc_11_b_rec(next, blink - 1);
            //printf("%s is zero\n", stone);
            sdsfree(next);
        }
        else if (sdslen(stone) % 2) {
            mpz_t c; mpz_init(c);
            char buf[512];
            mpz_set_str(c, stone, 10);
            mpz_mul_ui(c, c, 2024);
            mpz_get_str(buf, 10, c);
            mpz_clear(c);
            sds next = sdscat(sdsempty(), buf);
            //printf("%s * 2024 = %s\n", stone, next);
            res = aoc_11_b_rec(next, blink - 1);
            sdsfree(next);
        } else {
            int len = sdslen(stone) / 2, leading_zeroes = 0;
            sds l = sdscatlen(sdsempty(), stone, len);
            while (stone[len + leading_zeroes]
                    && stone[len + leading_zeroes] != '\n'
                    && stone[len + leading_zeroes] == '0')
                leading_zeroes++;
            leading_zeroes = min(leading_zeroes, len - 1);
            sds r = sdscatlen(sdsempty(), &stone[len + leading_zeroes], len - leading_zeroes);
            //printf("split %s: %s & %s\n", stone, l, r);
            res = aoc_11_b_rec(l, blink - 1)
                + aoc_11_b_rec(r, blink - 1);
            sdsfree(l);
            sdsfree(r);
        }
        //aoc11cacheput(stone_w_blink, res);
        shput(aoc_11_cache, stone_w_blink, res);
    }

    //printf("%s: %lu\n", stone, res);
    res = shget(aoc_11_cache, stone_w_blink);
    //res = aoc11cachefind(stone_w_blink);
    sdsfree(stone_w_blink);
    return res;
}

int aoc_11_a()
{
    u64 result = 0, blinks = 75;

    sds stones;
    sds *nums = NULL;

    char line[MAXLEN];
    FILE *f = fopen("resources/aoc_data/aoc_11_data.txt", "r");
    //FILE *f = fopen("aoc11datatest.txt", "r");
    while ((fgets(line, MAXLEN, f)) != NULL) {
        for (char *line_ptr = line; *line_ptr; ++line_ptr) {
            if (!isdigit(*line_ptr)) continue;
            int numlen = strnumlen(line_ptr);
            sds strnum = sdscatlen(sdsempty(), line_ptr, numlen);
            arrput(nums, strnum);
            line_ptr += numlen - 1;
        }
    } fclose(f);

    mpz_t res_mpz;
    mpz_init(res_mpz);
    mpz_set_ui(res_mpz, 0);
    for (int i = 0; i < arrlen(nums); ++i) {
        u64 res = aoc_11_b_rec(nums[i], blinks);
        mpz_add_ui(res_mpz, res_mpz, res);
        result += res;
        printf("%s: %lu\n", nums[i], res);
    }
    mpz_get_str(line, 10, res_mpz);
    printf("%s\n", line);

    printf("aoc_11_a: %lu\n", result);
    return result;
}










































int aoc_10_b_walk(int **map, ivec2_t pos)
{
    int result = 0;
    ivec2_t *stack = NULL;
    arrput(stack, pos);
    while (arrlen(stack)) {
        ivec2_t p = arrpop(stack);

        if (map[p.y][p.x] == 9) {
            result++;
            continue;
        }

        ivec2_t *nbs = neighbors(arrlen(map[0]), arrlen(map), p);
        for (int i = 0; i < arrlen(nbs); ++i) {
            int diff = map[p.y][p.x] - map[nbs[i].y][nbs[i].x];
            if (diff != -1)
                continue;
            arrput(stack, nbs[i]);
        }
        arrfree(nbs);
    }
    arrfree(stack);
    return result;
}

int aoc_10_b()
{
    u64 result = 0;
    int **map = NULL;
    char line[MAXLEN];
    FILE *f = fopen("resources/aoc_data/aoc_10_data.txt", "r");
    while ((fgets(line, MAXLEN, f)) != NULL) {
        arrput(map, NULL);
        char *line_ptr = line;
        while (*line_ptr && *line_ptr != '\n')
            arrput(map[arrlen(map) - 1], *line_ptr++ - '0');
    } fclose(f);

    for (int i = 0; i < arrlen(map); ++i) {
        for (int j = 0; j < arrlen(map[i]); ++j) {
            if (map[i][j] != 0)
                continue;
            ivec2_t pos = { j, i };
            int r = aoc_10_b_walk(map, pos);
            result += r;
            //printf("%d %d has score %d\n", j, i, r);
        }
    }



    printf("aoc_10_b: %lu\n", result);
    return result;
}

int aoc_10_a_walk(int **map, ivec2_t pos)
{
    int result = 0;
    ivec2_t *stack = NULL;
    ivec2_map_t *hmap = NULL;
    arrput(stack, pos);
    hmput(hmap, pos, pos);
    while (arrlen(stack)) {
        ivec2_t p = arrpop(stack);

        if (map[p.y][p.x] == 9) {
            result++;
            continue;
        }

        ivec2_t *nbs = neighbors(arrlen(map[0]), arrlen(map), p);
        for (int i = 0; i < arrlen(nbs); ++i) {
            if (hmgeti(hmap, nbs[i]) >= 0)
                continue;
            int diff = map[p.y][p.x] - map[nbs[i].y][nbs[i].x];
            if (diff != -1)
                continue;
            arrput(stack, nbs[i]);
            hmput(hmap, nbs[i], nbs[i]);
        }
        arrfree(nbs);
    }
    arrfree(stack);
    return result;
}

int aoc_10_a()
{
    u64 result = 0;
    int **map = NULL;
    char line[MAXLEN];
    FILE *f = fopen("resources/aoc_data/aoc_10_data.txt", "r");
    while ((fgets(line, MAXLEN, f)) != NULL) {
        arrput(map, NULL);
        char *line_ptr = line;
        while (*line_ptr && *line_ptr != '\n')
            arrput(map[arrlen(map) - 1], *line_ptr++ - '0');
    } fclose(f);

    for (int i = 0; i < arrlen(map); ++i) {
        for (int j = 0; j < arrlen(map[i]); ++j) {
            if (map[i][j] != 0)
                continue;
            ivec2_t pos = { j, i };
            int r = aoc_10_a_walk(map, pos);
            result += r;
        }
    }



    printf("aoc_10_b: %lu\n", result);
    return result;
}

int aoc_9_b()
{
    u64 result = 0;
    sds data = sdsfread(sdsempty(), "resources/aoc_data/aoc_9_data.txt");
    MemBlock *mem_blocks = NULL;

    int_map_t *id_seen = NULL;

    for (int i = 0; i < sdslen(data); ++i) {
        i32 id = (i % 2) ? -1 : i / 2;
        MemBlock mb = { .id = id, .space = data[i] - '0' };
        arrput(mem_blocks, mb);
    }

    for (int i = arrlen(mem_blocks) - 1; i >= 0; --i) {
        if (mem_blocks[i].id < 0 || hmgeti(id_seen, mem_blocks[i].id) >= 0)
            continue;
        for (int j = 0; j < i; ++j) {
            if (mem_blocks[j].id >= 0)
                continue;
            i32 mem_left = mem_blocks[j].space - mem_blocks[i].space;
            if (mem_left < 0)
                continue;

            hmput(id_seen, mem_blocks[i].id, mem_blocks[i].id);

            mem_blocks[j].id = mem_blocks[i].id;
            mem_blocks[j].space = mem_blocks[i].space;
            mem_blocks[i].id = -1;

            if (mem_left > 0) {
                MemBlock mb = { .id = -1, .space = mem_left };
                arrins(mem_blocks, j + 1, mb);
            }

            break;
        }
    }

    int off = 0;
    for (int i = 0; i < arrlen(mem_blocks); off += mem_blocks[i].space, ++i) {
        if (mem_blocks[i].id < 0)
            continue;
        for (int j = 0; j < mem_blocks[i].space; ++j) {
            result += mem_blocks[i].id * (j + off);
        }
    }

    sdsfree(data);
    arrfree(mem_blocks);
    printf("aoc_9_b: %lu\n", result);
    return result;
}

int aoc_9_a()
{
    u64 result = 0;
    sds data = sdsfread(sdsempty(), "resources/aoc_data/aoc_9_data.txt");
    i32 *data_exp = NULL;

    for (int i = 0; i < sdslen(data); ++i) {
        int len = data[i] - '0';
        int c = (i % 2) ? -1 : i / 2;
        for (int j = 0; j < len; ++j)
            arrput(data_exp, c);
    }

    int last_empty = 0;
    for (int i = arrlen(data_exp) - 1; i >= 0; --i) {
        if (data_exp[i] < 0)
            continue;
        for (int j = last_empty; j < i; ++j) {
            if (data_exp[j] < 0) {
                data_exp[j] = data_exp[i];
                data_exp[i] = -1;
                last_empty = j;
                break;
            }
        }
    }

    for (int i = 0; i < arrlen(data_exp); ++i) {
        if (data_exp[i] < 0)
            continue;
        result += data_exp[i] * i;
    }

    sdsfree(data);
    arrfree(data_exp);
    printf("aoc_9_a: %lu\n", result);
    return result;
}

int aoc_8_b()
{
    u64 result = 0;
    char **map = NULL, line[MAXLEN];

    FILE *f = fopen("resources/aoc_data/aoc_8_data.txt", "r");
    while ((fgets(line, MAXLEN, f)) != NULL) {
        arrput(map, NULL);
        char *line_ptr = line;
        while (*line_ptr && *line_ptr != '\n')
            arrput(map[arrlen(map) - 1], *line_ptr++);
    } fclose(f);
    /* init hmaps */
    antenna_map_t *antenna_map = NULL;
    for (int i = 0; i < arrlen(map); ++i) {
        for (int j = 0; j < arrlen(map[i]); ++j) {
            if (map[i][j] == '.')
                continue;
            ivec2_t p = { j, i };
            if (hmgeti(antenna_map, map[i][j]) < 0)
                hmput(antenna_map, map[i][j], NULL);
            antenna_map_t *am = hmgetp(antenna_map, map[i][j]);
            hmput(am->value, p, p);
        }
    }
    /* find antinodes */
    hmput(antenna_map, '#', NULL);
    antenna_map_t *antinode_map = hmgetp(antenna_map, '#');
    for (int i = 0; i < hmlen(antenna_map); ++i) {
        if (antenna_map[i].key == '#') continue;
        ivec2_map_t *ant_list = antenna_map[i].value;
        for (int j = 0; j < hmlen(ant_list); ++j) {
            for (int k = j + 1; k < hmlen(ant_list); ++k) {
                ivec2_t p1 = ant_list[j].key;
                ivec2_t p2 = ant_list[k].key;

                ivec2_t step = { p1.x - p2.x, p1.y - p2.y };
                ivec2_t pos;

                pos = p1;
                while (pos.x >= 0 && pos.x < arrlen(map[0])
                        && pos.y >= 0 && pos.y < arrlen(map)) {
                    hmput(antinode_map->value, pos, pos);
                    pos.x += step.x;
                    pos.y += step.y;
                }

                pos = p2;
                while (pos.x >= 0 && pos.x < arrlen(map[0])
                        && pos.y >= 0 && pos.y < arrlen(map)) {
                    hmput(antinode_map->value, pos, pos);
                    pos.x -= step.x;
                    pos.y -= step.y;
                }
            }
        }
    }

    result = hmlen(antinode_map->value);
    printf("aoc_8_a: %lu\n", result);
    return result;
}

int aoc_8_a()
{
    u64 result = 0;
    char **map = NULL, line[MAXLEN];

    //FILE *f = fopen("aoc8datatest.txt", "r");
    FILE *f = fopen("resources/aoc_data/aoc_8_data.txt", "r");
    while ((fgets(line, MAXLEN, f)) != NULL) {
        arrput(map, NULL);
        char *line_ptr = line;
        while (*line_ptr && *line_ptr != '\n')
            arrput(map[arrlen(map) - 1], *line_ptr++);
    } fclose(f);
    /* init hmaps */
    antenna_map_t *antenna_map = NULL;
    for (int i = 0; i < arrlen(map); ++i) {
        for (int j = 0; j < arrlen(map[i]); ++j) {
            if (map[i][j] == '.')
                continue;
            ivec2_t p = { j, i };
            if (hmgeti(antenna_map, map[i][j]) < 0)
                hmput(antenna_map, map[i][j], NULL);
            antenna_map_t *am = hmgetp(antenna_map, map[i][j]);
            hmput(am->value, p, p);
        }
    }
    /* find antinodes */
    hmput(antenna_map, '#', NULL);
    antenna_map_t *antinode_map = hmgetp(antenna_map, '#');
    for (int i = 0; i < hmlen(antenna_map); ++i) {
        if (antenna_map[i].key == '#') continue;
        ivec2_map_t *ant_list = antenna_map[i].value;
        for (int j = 0; j < hmlen(ant_list); ++j) {
            for (int k = j + 1; k < hmlen(ant_list); ++k) {
                ivec2_t p1 = ant_list[j].key;
                ivec2_t p2 = ant_list[k].key;

                int dx = abs(p1.x - p2.x);
                int dy = abs(p1.y - p2.y);
                int sign_x = p1.x < p2.x ? -1 : 1;
                int sign_y = p1.y < p2.y ? -1 : 1;

                ivec2_t a1 = { p1.x + dx * sign_x, p1.y + dy * sign_y};
                ivec2_t a2 = { p2.x + dx * -sign_x, p2.y + dy * -sign_y};

                //printf("adding antinode (%c) at %d %d (near: %d %d far: %d %d)\n",
                       //antenna_map[i].key, a1.x, a1.y, p1.x, p1.y, p2.x, p2.y);
                //printf("adding antinode (%c) at %d %d (near: %d %d far: %d %d)\n",
                       //antenna_map[i].key, a2.x, a2.y, p2.x, p2.y, p1.x, p1.y);

                hmput(antinode_map->value, a1, a1);
                hmput(antinode_map->value, a2, a2);
            }
        }
    }
    /* print hmaps */
    //for (int i = 0; i < hmlen(antenna_map); ++i) {
        //printf("%c: [", antenna_map[i].key);
        //for (int j = 0; j < hmlen(antenna_map[i].value); ++j) {
            //printf("(%d,%d)", antenna_map[i].value[j].value.x, antenna_map[i].value[j].value.y);
        //}
        //printf("]\n");
    //}
    /* print map */
    //for (int i = 0; i < arrlen(map); ++i) {
        //for (int j = 0; j < arrlen(map[i]); ++j) {
            //ivec2_t p = { j, i };
            //if (hmgeti(antinode_map->value, p) >= 0) {
                //result++;
                //if (map[i][j] == '.')
                    //printf("%c", antinode_map->key);
                //else
                    //printf("%c", map[i][j]);
            //}
            //else
                //printf("%c", map[i][j]);
        //}
        //printf("\n");
    //}

    printf("aoc_8_a: %lu\n", result);

    return result;
}

bool aoc_7_b_2_help(u64 answer, u64 current, u64 *terms, int idx)
{
    if (idx >= arrlen(terms) || current > answer) {
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
    char line[400];
    while ((fgets(line, 400, f)) != NULL) {
        arrput(eqs, NULL);
        char *line_ptr = line;
        while (*line_ptr && *line_ptr != '\n') {
            if (isdigit(*line_ptr)) {
                i64 ll = strtoll(line_ptr, &line_ptr, 10);
                assert(ll != LLONG_MAX);
                assert(ll != LLONG_MIN);
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

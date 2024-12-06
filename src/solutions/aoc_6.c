/*****************************************************
Create Date:        2024-12-01
Author:             Oskar Bahner Hansen
Email:              cph-oh82@cphbusiness.dk
Description:        exercise template
License:            none
*****************************************************/

#include "../../include/obh/incl.h"
#define STB_DS_IMPLEMENTATION
#include "../../include/stb/stb_ds.h"

typedef struct ivec2_t {
    int x, y;
} ivec2_t;

typedef struct ivec2_map_t {
    union {
        ivec2_t key;
        ivec2_t value;
    };
} ivec2_map_t;

int aoc_6_b()
{
    i64 result = 0;
    sds *data = NULL;

    i8 dir;
    char *dir_chars = "^>v<";
    ivec2_t dirs[] = { {  0, -1 }, {  1,  0 }, {  0,  1 }, { -1,  0 } };
    ivec2_t pos;

    ivec2_map_t *turns = NULL;

    char line[4000];
    while ((fgets(line, 4000, stdin)) != NULL) {
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

            //printf("%.*s\n", (int)sdslen(data[0]), "--------------------------------------------");
            //for (int i = 0; i < arrlen(data); ++i)
                //printf("%s\n", data[i]);
            //printf("DONE\n");

            break;
        }

        if (data[pos.y][pos.x] == '#') {
            pos.x -= dirs[dir].x;
            pos.y -= dirs[dir].y;
            data[pos.y][pos.x] = dir_chars[dir];
            hmput(turns, pos, pos);
            dir = (dir + 1) % 4;
            continue;
        }

        data[pos.y][pos.x] = dir_chars[dir];


    }

    for (int i = 0; i < arrlen(data); ++i) {
        for (int j = 0; j < sdslen(data[i]); ++j) {
            ivec2_t p = { j, i };
            if (hmgeti(turns, p) >= 0)
                data[i][j] = 'T';
        }
    }

    for (int i = 0; i < sdslen(data[0]); ++i)
        printf("-");
    printf("\n");
    for (int i = 0; i < arrlen(data); ++i)
        printf("%s\n", data[i]);
    for (int i = 0; i < sdslen(data[0]); ++i)
        printf("-");
    printf("\n");

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

    char line[4000];
    while ((fgets(line, 4000, stdin)) != NULL) {
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

            //printf("%.*s\n", (int)sdslen(data[0]), "--------------------------------------------");
            //for (int i = 0; i < arrlen(data); ++i)
                //printf("%s\n", data[i]);
            //printf("DONE\n");

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

    return result;
}

int main(int argc, char *argv[])
{
    printf("%d\n", aoc_6_a());
    printf("%d\n", aoc_6_b());
    return 0;
}

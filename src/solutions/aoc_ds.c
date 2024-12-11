typedef struct ivec2_t {
    int x, y;
} ivec2_t;

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

typedef struct ivec2_map_t {
    union {
        ivec2_t key;
        ivec2_t value;
    };
} ivec2_map_t;

typedef struct antenna_map_t {
    char key;
    ivec2_map_t *value;
} antenna_map_t;

typedef struct MemBlock {
    i32 id, space;
} MemBlock;

void aoc_9_print_mb(MemBlock *mbs)
{
    for (int i = 0; i < arrlen(mbs); ++i) {
        if (mbs[i].id < 0) {
            for (int j = 0; j < mbs[i].space; ++j) {
                printf("[.]");
            }
        }
        else {
            for (int j = 0; j < mbs[i].space; ++j) {
                printf("[%d]", mbs[i].id);
            }
        }
    }
    printf("\n");
}

void aoc_9_print(i32 *data_exp)
{
    for (int i = 0; i < arrlen(data_exp); ++i) {
        if (data_exp[i] < 0)
            printf(".");
        else
            printf("%d", data_exp[i]);
    }
    printf("\n");
}
/**
 * returns heap allocated array of in-bounds neighbor positions to pos.
 */
ivec2_t *neighbors(int w, int h, ivec2_t pos)
{
    ivec2_t *nbs = NULL;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            int x = pos.x + j;
            int y = pos.y + i;
            if (x == pos.x && y == pos.y)
                continue;
            if ((x != pos.x && y != pos.y))
                continue;
            ivec2_t p = { x, y };
            if (x >= 0 && x < w && y >= 0 && y < h)
                arrput(nbs, p);
        }
    }
    return nbs;
}

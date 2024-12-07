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



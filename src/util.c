#include "../include/obh/util.h"

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


void arr_i_print(const int *arr, const int len)
{
    int max = 0;
    for (int i = 0; i < len; ++i)
        if (abs(arr[i]) > max)
            max = abs(arr[i]);
    const int digits = (int)log10(max + 1) + 2;
    printf("[");
    for (int i = 0; i < len; ++i) {
        printf("%*d", digits, arr[i]);
        if (i < len - 1)
            printf(",");
    }
    printf("]");
}

void arr_i_print2d(const int *arr, const int width, const int height)
{
    int max = 0;
    for (int i = 0; i < width * height; ++i)
        if (abs(arr[i]) > max)
            max = abs(arr[i]);
    const int digits = (int)log10(max + 1) + 2;
    printf("[");
    for (int i = 0; i < width * height; ++i) {
        printf("%*d", digits, arr[i]);
        if (i < width - 1)
            printf(",");
        if (i % width == 0 && i != 0)
            printf("\n");
    }
    printf("]");
}

void arr_f_print(double *arr, int len)
{
    int max = 0;
    for (int i = 0; i < len; ++i)
        if (fabs(arr[i]) > max)
            max = fabs(arr[i]);
    int digits = (int)log10(max + 1) + 2;
    printf("[");
    for (int i = 0; i < len; ++i) {
        printf("%*f", digits, arr[i]);
        if (i < len - 1)
            printf(",");
    }
    printf("]");
}

sds sdsfread(sds append_to, const char* path)
{
    size_t old_len = sdslen(append_to);

    FILE *fptr = fopen(path, "r");
    if (fptr == NULL) { c_log_error(LOG_TAG, "%s", strerror(errno)); }
    fseek(fptr, 0, SEEK_END);
    size_t n_read = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    append_to = sdsgrowzero(append_to, sdslen(append_to) + n_read);
    size_t n_written = fread(&append_to[old_len], 1, n_read, fptr);
    if (n_read != n_written) { c_log_error(LOG_TAG, "%llu != %llu", n_read, n_written); }

    fclose(fptr);
    append_to[sdslen(append_to)] = 0;
    return append_to;
}



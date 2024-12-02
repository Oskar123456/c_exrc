/*****************************************************
Create Date:        2024-11-12
Author:             Oskar Bahner Hansen
Email:              cph-oh82@cphbusiness.dk
Description:        exercise template
License:            none
*****************************************************/

#include "../include/obh/incl.h"
#include "../include/obh/c_log.h"
#include "../include/obh/calc.h"

#include "solutions/solution.c"
#include "solutions/advent_of_code.c"

#define STB_DS_IMPLEMENTATION
#include "../include/stb/stb_ds.h"

int var_arg_test(int n, ...)
{
    int res = 0;
    printf("%d: %d (%p)\n", -1, n, &n);
    char *arg_ptr = (char*)(&n) + 28;
    for (int i = 0; i < (n > 5 ? 5 : n); ++i) {
        printf("%d: %d (%p)\n", i, *(int*)arg_ptr, arg_ptr);
        arg_ptr += 8;
    }
    arg_ptr = (char*)(&n) + 212;
    for (int i = 0; i < n - 5; ++i) {
        printf("%d: %d (%p)\n", i, *(int*)arg_ptr, arg_ptr);
        arg_ptr += 8;
    }
    return res;
}

void var_arg_test_lol(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10)
{
    int i = 0;
    printf("%d: (%p)\n", i++, &n1);
    printf("%d: (%p)\n", i++, &n2);
    printf("%d: (%p)\n", i++, &n3);
    printf("%d: (%p)\n", i++, &n4);
    printf("%d: (%p)\n", i++, &n5);
    printf("%d: (%p)\n", i++, &n6);
    printf("%d: (%p)\n", i++, &n7);
    printf("%d: (%p)\n", i++, &n8);
    printf("%d: (%p)\n", i++, &n9);
    printf("%d: (%p)\n", i++, &n10);
}

int va_list_test(int n, ...)
{
    int c_arg, i = 0, res = 0;
    va_list ap;
    va_start(ap, n);
    for (int i = 0; i < n; ++i)
        res += va_arg(ap, int);
    va_end(ap);
    return res;
}


int main(int argc, char *argv[])
{

    int exit_code = 0;
    /* initialization */
    srand(time(NULL));
    c_log_init(stderr, LOG_LEVEL_SUCCESS);

    int delim_len = 50;
    char open[delim_len + 1], close[delim_len + 1];
    for (int i = 0; i < delim_len; ++i) {
        open[i] = '>'; close[i] = '<';
    }
    open[delim_len] = 0; close[delim_len] = 0;

    printf("%s\n", open);

    int maxlen = 256;
    char line[maxlen];
    //while ((fgets(line, maxlen, stdin)) != NULL) {
        //lex(line);
        //SyntaxTree* ast = parse();
        //printast(ast, 0);
        //c_log_success(LOG_TAG, "result: %f", eval(ast));
        //double res = calcEval(line);
        //c_log_success(LOG_TAG, "result: %f", res);
    //}

    printf(" >>> advent of code day 1 (a) solution: %d \u2713\n", aoc_1_a());
    printf(" >>> advent of code day 1 (b) solution: %d \u2713\n", aoc_1_b());

    printf("%s\n", close);
    return exit_code;

}

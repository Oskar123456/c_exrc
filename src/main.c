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

int main(int argc, char *argv[])
{

    int exit_code = 0;
    /* initialization */
    srand(time(NULL));
    c_log_init(stderr, LOG_LEVEL_SUCCESS);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int delim_len = w.ws_col - 1;
    char open[delim_len + 1], close[delim_len + 1];
    for (int i = 0; i < delim_len; ++i) {
        open[i] = '>'; close[i] = '<';
    }
    open[delim_len] = 0; close[delim_len] = 0;

    printf("%s\n", open);

    /* LET THE GAMES BEGIN */

    printf("***ADVENT OF CODE 2024***\n");
    printf(">>> advent of code day 1\n");
    struct timespec t, at;
    u64 dt_ns, dt_sec, dt_millis;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    int aoc1a = aoc_1_a();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &at);
    dt_ns  = at.tv_nsec - t.tv_nsec;
    dt_sec = at.tv_sec - t.tv_sec;
    u64 aoc1a_dt_micro = dt_ns / 1000 + dt_sec * 1000000;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    int aoc1b = aoc_1_b();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &at);
    dt_ns  = at.tv_nsec - t.tv_nsec;
    dt_sec = at.tv_sec - t.tv_sec;
    u64 aoc1b_dt_micro = dt_ns / 1000 + dt_sec * 1000000;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    int aoc2a = aoc_2_a();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &at);
    dt_ns  = at.tv_nsec - t.tv_nsec;
    dt_sec = at.tv_sec - t.tv_sec;
    u64 aoc2a_dt_micro = dt_ns / 1000 + dt_sec * 1000000;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    int aoc2b = aoc_2_b();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &at);
    dt_ns  = at.tv_nsec - t.tv_nsec;
    dt_sec = at.tv_sec - t.tv_sec;
    u64 aoc2b_dt_micro = dt_ns / 1000 + dt_sec * 1000000;

    printf("\t(a) solution: %d (in %lumcs) (\u2713)\n", aoc1a, aoc1a_dt_micro);
    printf("\t(b) solution: %d (in %lumcs) (\u2713)\n", aoc1b, aoc1b_dt_micro);

    printf(">>> advent of code day 2\n");
    printf("\t(a) solution: %d (in %lumcs) (\u2713)\n", aoc2a, aoc2a_dt_micro);
    printf("\t(b) solution: %d (in %lumcs) (\u2713)\n", aoc2b, aoc2b_dt_micro);

    //int maxlen = 256;
    //char line[maxlen];
    //while ((fgets(line, maxlen, stdin)) != NULL) {
        //lex(line);
        //SyntaxTree* ast = parse();
        //printast(ast, 0);
        //c_log_success(LOG_TAG, "result: %f", eval(ast));
        //double res = calcEval(line);
        //c_log_success(LOG_TAG, "result: %f", res);
    //}

    /* LET THE GAMES END */

    printf("%s\n", close);
    return exit_code;

}

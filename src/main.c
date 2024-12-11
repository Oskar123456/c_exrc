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
#include <pcre2.h>

#define STB_DS_IMPLEMENTATION
#include "../include/stb/stb_ds.h"

void timeFn(int (*fn)(void), int *dest, u64 *time_dest)
{
    struct timespec t, at;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    *dest = fn();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &at);
    u64 t_mcs = t.tv_sec * 1000000 + t.tv_nsec / 1000;
    u64 at_mcs = at.tv_sec * 1000000 + at.tv_nsec / 1000;
    *time_dest = at_mcs - t_mcs; /* micros */
}

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
    int result_int; u64 time_mcs;

    printf("***ADVENT OF CODE 2024***\n");

    //printf(">>> advent of code day 1\n");
    //timeFn(aoc_1_a, &result_int, &time_mcs);
    //printf("\t(a) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);
    //timeFn(aoc_1_b, &result_int, &time_mcs);
    //printf("\t(b) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);

    //printf(">>> advent of code day 2\n");
    //timeFn(aoc_2_a, &result_int, &time_mcs);
    //printf("\t(a) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);
    //timeFn(aoc_2_b, &result_int, &time_mcs);
    //printf("\t(b) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);

    //printf(">>> advent of code day 3\n");
    //timeFn(aoc_3_a, &result_int, &time_mcs);
    //printf("\t(a) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);
    //timeFn(aoc_3_b, &result_int, &time_mcs);
    //printf("\t(b) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);

    //printf(">>> advent of code day 4\n");
    //timeFn(aoc_4_a, &result_int, &time_mcs);
    //printf("\t(a) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);
    //timeFn(aoc_4_b, &result_int, &time_mcs);
    //printf("\t(b) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);

    //printf(">>> advent of code day 5\n");
    //timeFn(aoc_5_a, &result_int, &time_mcs);
    //printf("\t(a) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);
    //timeFn(aoc_5_b, &result_int, &time_mcs);
    //printf("\t(b) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);

    //printf(">>> advent of code day 6\n");
    //timeFn(aoc_6_a, &result_int, &time_mcs);
    //printf("\t(a) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);
    //timeFn(aoc_6_b, &result_int, &time_mcs);
    //printf("\t(b) solution: %d (in %lumcs) (\u2713)\n", result_int, time_mcs);

    //printf(">>> advent of code day 7\n");
    //timeFn(aoc_7_a_2, &result_int, &time_mcs);
    //printf("\t(a) in %lumcs (\u2713)\n", time_mcs);
    //timeFn(aoc_7_b_2, &result_int, &time_mcs);
    //printf("\t(b) in %lumcs (\u2713)\n", time_mcs);

    //printf(">>> advent of code day 8\n");
    //timeFn(aoc_8_a, &result_int, &time_mcs);
    //printf("\t(a) in %lumcs (\u2713)\n", time_mcs);
    //timeFn(aoc_8_b, &result_int, &time_mcs);
    //printf("\t(a) in %lumcs (\u2713)\n", time_mcs);

    //printf(">>> advent of code day 9\n");
    //timeFn(aoc_9_a, &result_int, &time_mcs);
    //printf("\t(a) in %lumcs (\u2713)\n", time_mcs);
    //timeFn(aoc_9_b, &result_int, &time_mcs);
    //printf("\t(b) in %lumcs (\u2713)\n", time_mcs);

    //printf(">>> advent of code day 10\n");
    //timeFn(aoc_10_a, &result_int, &time_mcs);
    //printf("\t(a) in %lumcs (\u2713)\n", time_mcs);
    //timeFn(aoc_10_b, &result_int, &time_mcs);
    //printf("\t(b) in %lumcs (\u2713)\n", time_mcs);


    printf(">>> advent of code day 11\n");
    timeFn(aoc_11_a, &result_int, &time_mcs);
    printf("\t(a) in %lumcs (\u2713)\n", time_mcs);
    /* LET THE GAMES END */

    printf("%s\n", close);
    return exit_code;

}

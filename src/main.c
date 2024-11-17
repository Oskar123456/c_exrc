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

//#include "solution.c"

#define STB_DS_IMPLEMENTATION
#include "../include/stb/stb_ds.h"

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

    calcTest(10);

    int maxlen = 256;
    char line[maxlen];
    while ((fgets(line, maxlen, stdin)) != NULL) {
        //lex(line);
        //SyntaxTree* ast = parse();
        //printast(ast, 0);
        //c_log_success(LOG_TAG, "result: %f", eval(ast));
        double res = calcEval(line);
        c_log_success(LOG_TAG, "result: %f", res);
    }

    printf("%s\n", close);
    return exit_code;

}

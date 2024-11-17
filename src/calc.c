/*****************************************************
Create Date:        2024-11-12
Author:             Oskar Bahner Hansen
Email:              cph-oh82@cphbusiness.dk
Description:        exercise template
License:            none
*****************************************************/

#include "../include/obh/calc.h"

double calcUseOp(char op, double a, double b)
{
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            c_log_error(LOG_TAG, "%c not an op", op);
            return 0;
    }
}

int calcGetOpPrec(char op)
{
    switch (op) {
        case '+':
            return 2;
        case '-':
            return 2;
        case '*':
            return 1;
        case '/':
            return 1;
        default:
            return 0;
    }
}

bool calcIsOp(char op)
{
    switch (op) {
        case '+':
            return true;
        case '-':
            return true;
        case '*':
            return true;
        case '/':
            return true;
        default:
            return false;
    }
}

double calcEvalInternal(char *expr, int prec_level)
{
    if (prec_level < 0) {
        c_log_error(LOG_TAG, "prec_level < 0 (%d)", prec_level);
        return NAN;
    }

    c_log_info(LOG_TAG, "eval %s (pl:%d)", expr, prec_level);
    //sleep(1);

    char prev = 0;
    char *expr_ptr = expr;
    for (; *expr_ptr != 0; expr_ptr++) {
        char c = *expr_ptr;

        if (isspace(c))
            continue;

        if (prec_level == 0) {
            if (isdigit(c) || c == '-') {
            char *num_endptr;
            double num = strtod(expr_ptr, &num_endptr);
            if (num_endptr == expr_ptr || num_endptr == NULL)
                c_log_error(LOG_TAG, "number format error at %ld", expr_ptr - expr);
            else
                c_log_info(LOG_TAG, "number constant %f at %ld", num, expr_ptr - expr);
            return num;
            }
        }

        if (calcIsOp(c)) {
            int op_prec = calcGetOpPrec(c);
            if (op_prec < 1)
                c_log_error(LOG_TAG, "invalid op %c", c);
            if (op_prec == prec_level) {
                return calcUseOp(c,
                        calcEvalInternal(expr, prec_level - 1),
                        calcEvalInternal(expr_ptr + 1, prec_level));
            }
            if (op_prec > prec_level)
                break;
        }
    }

    return calcEvalInternal(expr, prec_level - 1);
}

double calcEval(char *expr)
{
    return calcEvalInternal(expr, 2);
}

void calcTest(int n)
{
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        int  n_nums = (rand() % 5) + 4;
        double  nums[n_nums];
        char ops[n_nums - 1];
        char ops_opts[] = "+-*/";

        for (int j = 0; j < n_nums; ++j) {
            nums[j] = ((double)(rand() % 1000)); // + (double)rand() / RAND_MAX) / 3;
            if (j < n_nums - 1)
                ops[j] = ops_opts[rand() % strlen(ops_opts)];
        }

        sds expr_str = sdsempty();
        for (int j = 0; j < n_nums; ++j) {
            expr_str = sdscatprintf(expr_str, "%f", nums[j]);
            if (j < n_nums - 1)
                expr_str = sdscatprintf(expr_str, " %c ", ops[j]);
        }

        printf("%s = %f\n", expr_str, calcEval(expr_str));
        sdsfree(expr_str);
    }
}


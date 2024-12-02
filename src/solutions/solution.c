/*****************************************************
Create Date:        2024-11-12
Author:             Oskar Bahner Hansen
Email:              cph-oh82@cphbusiness.dk
Description:        exercise template
License:            none
*****************************************************/

#include "../../include/obh/incl.h"
#include "../../include/obh/c_log.h"
#include "../../include/stb/stb_ds.h"

typedef enum TOK_TYPE {
    TOK_OP,
    TOK_NUM,
} TOK_TYPE;

typedef struct Token {
    TOK_TYPE type;
    char     val_chr;
    int      val_int;
    double   val_dbl;
} Token;

typedef struct SyntaxTree {
    Token *this;
    struct SyntaxTree *parent, *child_l, *child_r;
} SyntaxTree;

static Token* tokens;
static SyntaxTree* ast;
static int ast_root_idx;

void printtokenshort(Token t)
{
    if (t.type == TOK_NUM)
        printf("%.2f", t.val_dbl);
    if (t.type == TOK_OP)
        printf("%c", t.val_chr);
}

void printtoken(Token t)
{
    printf("Token[");
    if (t.type == TOK_NUM)
        printf("NUM %.2f", t.val_dbl);
    if (t.type == TOK_OP)
        printf("OP %c", t.val_chr);
    printf("]");
}

void printast(SyntaxTree *root, int depth)
{
    for (int i = 0; i < depth; ++i) {
        printf("\t");
    }
    printtokenshort(*root->this);
    printf("\n");
    if (root->child_l)
        printast(root->child_l, depth + 1);
    if (root->child_r)
        printast(root->child_r, depth + 1);

}

int opprecedence(char c)
{
    if (c == '*' || c == '/')
        return 2;
    if (c == '+' || c == '-')
        return 1;
    return 0;
}

void lex(char* expr)
{
    arrfree(tokens);
    tokens = NULL;
    for (char* expr_ptr = expr; *expr_ptr != 0; expr_ptr++) {
        char c = *expr_ptr;
        if (isdigit(c)) {
            Token t = { .type = TOK_NUM, .val_dbl = strtod(expr_ptr, &expr_ptr) };
            arrput(tokens, t);
        }
        if (ispunct(c)) {
            Token t = { .type = TOK_OP, .val_chr = c };
            arrput(tokens, t);
        }
    }
}

SyntaxTree* parse()
{
    arrfree(ast);
    ast = NULL;
    ast_root_idx = 0;

    SyntaxTree ast_node = { .this = &tokens[0] };
    arrput(ast, ast_node);
    for (int i = 1; i < arrlen(tokens); i++) {
        ast_node = (SyntaxTree) { .this = &tokens[i] };
        arrput(ast, ast_node);

        SyntaxTree *prev = &ast[arrlen(ast) - 2];
        SyntaxTree *curr = &ast[arrlen(ast) - 1];
        SyntaxTree *root = &ast[ast_root_idx];

        if (ast_node.this->type == TOK_NUM) {
            root->child_r = curr;
            curr->parent = root;
        }

        if (ast_node.this->type == TOK_OP) {
            if (prev->parent) {
                if (prev->parent->this->type == TOK_OP) {
                    if (opprecedence(prev->parent->this->val_chr)
                            < opprecedence(curr->this->val_chr)) {
                    }
                }
            }
            curr->child_l = root;
            root->parent = curr;
            ast_root_idx = arrlen(ast) - 1;
        }

    }

    return &ast[ast_root_idx];
}

double eval(SyntaxTree *root)
{
    if (!root)
        return 0;
    if (root->this->type == TOK_NUM)
        return root->this->val_dbl;
    if (root->this->type == TOK_OP) {
        if (root->this->val_chr == '+')
            return (eval(root->child_l) + eval(root->child_r));
        if (root->this->val_chr == '-')
            return (eval(root->child_l) - eval(root->child_r));
        if (root->this->val_chr == '*')
            return (eval(root->child_l) * eval(root->child_r));
        if (root->this->val_chr == '/')
            return (eval(root->child_l) / eval(root->child_r));
    }
    return 0;
}


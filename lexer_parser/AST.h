#ifndef AST_H
#define AST_H

#include "Lexer.h"

typedef enum {
    NODE_NUM, 
    NODE_VAR,
    NODE_NEG, 
    NODE_BINARY 
} NodeKind;

typedef struct Node {
    NodeKind kind;
    union {
        int value; //NODE_NUM
        Token name; //NODE_VAR
        struct {
            char op; // NODE_BINARY: '+', '-', '*', '/'
            struct Node *left, *right; // NODE_BINARY uses both, NODE_NEG uses left
        } binary;
    };

    struct {
        Token var_name;     // The variable getting the value
        struct Node *expr;  // The expression value being assigned
    } assign;               // NODE_ASSIGN

} Node;


typedef struct {
    Lexer lx;
    Token current;   // the next token, not yet consumed (the "lookahead")
    Token previous;  // the token we just consumed
} Parser;

Node *parse_expression(const char *source);

#endif


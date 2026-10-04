#include "Lexer.h"
#include "AST.h"

static Node *new_node(NodeKind kind) {
    Node *instance = calloc(1, sizeof(Node));
    instance->kind = kind; 
    return instance;
}

static Node *new_binary(char op, Node *left, Node *right) {
    Node *n = new_node(NODE_BINARY);
    n->op = op; n->left = left, n->right = right;
    return n;
}

static void error_at(Token t, const char *msg) {
    fprintf();
}

static void parser_advance(Parser *p){
    p->previous = p->current; 
    p->current = next_token()
}

static void error_at(Token t, const char *msg) {

}

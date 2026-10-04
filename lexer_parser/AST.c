#include "Lexer.h"
#include "AST.h"
#include "stdio.h"
#include <stdlib.h>

static Node *new_node(NodeKind kind) {
    Node *instance = calloc(1, sizeof(Node));
    instance->kind = kind; 
    return instance;
}

static Node *new_binary(char op, Node *left, Node *right) {
    Node *n = new_node(NODE_BINARY);
    n->binary.op = op;
    n->binary.left = left;
    n->binary.right = right;
    return n;
}

static void error_at(Token t, const char *msg) {
    fprintf(stderr, "line %d: %s", t.line, msg);
    if (t.type == TOK_EOF) fprintf(stderr, " at end\n");
    else                   fprintf(stderr, " at '%.*s'\n", t.length, t.start);
    exit(1);
}

static void parser_advance(Parser *p) {
    p->previous = p->current;
    p->current = next_token(&p->lx);
    if (p->current.type == TOK_ERROR) {
        fprintf(stderr, "line %d: %.*s\n", p->current.line,
                p->current.length, p->current.start);
        exit(1);
    }
}

static int check(Parser *p, TokenType type) { return p->current.type == type; }

static int match(Parser *p, TokenType type) {
    if (!check(p, type)) return 0;
    parser_advance(p);
    return 1;
}

static void expect(Parser *p, TokenType type, const char *msg) {
    if (!match(p, type)) error_at(p->current, msg);
}

static Node *expr(Parser *p); 

static Node *primary(Parser *p) {
    if(match(p, TOK_NUMBER)) {
        Node *n = new_node(NODE_NUM);
        for (int i = 0; i < p->previous.length; i++)
        {
            n->value = n->value*10 + (p->previous.start[i] - '0');
        }
        return n;
    }

    if(match(p, TOK_IDENT)) {
        Node *n = new_node(NODE_VAR);
        n->name = p->previous;
        return n;
    }

    if (match(p, TOK_LPAREN)) {
        Node *n = expr(p);                         // recurse all the way back up because when it is a bracket we lead to recurssion
        expect(p, TOK_RPAREN, "Expected ')'");
        return n;                                  // the parens vanish
    }
    error_at(p->current, "Expected expression");
    return NULL;
}

static Node *unary(Parser *p) {
    if(match(p, TOK_MINUS)) { // why are we only taking into account the - ?
        Node *n = new_node(NODE_NEG); 
        n->binary.left = unary(p);
        return n;
    }
    return primary(p);
}

static Node *term(Parser *p) {
    Node *left = unary(p);
    while (check(p, TOK_STAR) || check(p, TOK_SLASH)) {
        char op = *p->current.start;   // '*' or '/'
        parser_advance(p);
        Node *right = unary(p);
        left = new_binary(op, left, right);
    }
    return left;
}

static Node *expr(Parser *p) {
    Node *left = term(p);
    while (check(p, TOK_PLUS) || check(p, TOK_MINUS)) {
        char op = *p->current.start;
        parser_advance(p);
        Node *right = term(p);
        left = new_binary(op, left, right);
    }
    return left;
}

Node *parse_expression(const char *source) {
    Parser parser = {0};
    lexer_init(&parser.lx, source);
    parser_advance(&parser);

    Node *expression = expr(&parser);
    expect(&parser, TOK_EOF, "Expected end of input");
    return expression;
}
#include <stdio.h>
#include "Lexer.h"
#include "AST.h"

static void print_ast(Node *n) {
    switch (n->kind) {
        case NODE_NUM: printf("%d", n->value); break;
        case NODE_VAR: printf("%.*s", n->name.length, n->name.start); break;
        case NODE_NEG: printf("(neg "); print_ast(n->binary.left); printf(")"); break;
        case NODE_BINARY:
            printf("(%c ", n->binary.op);
            print_ast(n->binary.left); printf(" ");
            print_ast(n->binary.right); printf(")");
            break;
    }
}

int main(void) {
    const char *src = "let x = 3 + 4 * (2 - 1);\nprint x * 2;";
    Lexer lx;
    lexer_init(&lx, src);
    for (;;) {
        Token t = next_token(&lx);
        printf("%d  %-7s '%.*s'\n", t.line, token_type_names[t.type], t.length, t.start);
        if (t.type == TOK_EOF || t.type == TOK_ERROR) break;
    }

    const char *src2 = "3 + 4 * (2 - 1)";
    Node *tree = parse_expression(src2);
    print_ast(tree);
    printf("\n");

}
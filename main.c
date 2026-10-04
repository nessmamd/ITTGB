#include <stdio.h>
#include "Lexer.h"

int main(void) {
    const char *src = "let x = 3 + 4 * (2 - 1);\nprint x * 2;";
    Lexer lx;
    lexer_init(&lx, src);
    for (;;) {
        Token t = next_token(&lx);
        printf("%d  %-7s '%.*s'\n", t.line, token_type_names[t.type], t.length, t.start);
        if (t.type == TOK_EOF || t.type == TOK_ERROR) break;
    }
}
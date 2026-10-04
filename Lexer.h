#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOK_NUMBER,
    TOK_IDENT,
    TOK_LET,
    TOK_PRINT,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_EQUAL,
    TOK_SEMI,
    TOK_EOF,
    TOK_ERROR
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int length;
    int line;
} Token;

typedef struct {
    const char *start;
    const char *current;
    int line;
} Lexer;

extern const char *const token_type_names[];

void lexer_init(Lexer *lx, const char *source);
Token next_token(Lexer *lx);

#endif
#include "Lexer.h"

#include <ctype.h>
#include <string.h>

const char *const token_type_names[] = {
    "NUMBER", "IDENT", "LET", "PRINT",
    "PLUS", "MINUS", "STAR", "SLASH",
    "LPAREN", "RPAREN", "EQUAL", "SEMI",
    "EOF", "ERROR"
};

// STEP 1: Create a token
static int is_at_end(Lexer *lx) { return *lx->current == '\0' ;}
static char advance(Lexer *lx) { return *lx->current++; } 
static char peek(Lexer *lx) {return *lx->current; }

static Token make_token(Lexer *lx, TokenType type) {
    // this is the towards the end of each word, we take the current and then make it into an 
    // instance of a token
    Token t = {type, lx->start, (int)(lx->current - lx->start), lx->line}; 
    return t;
}

static Token error_token(Lexer *lx, const char *msg) {
    Token t = {TOK_ERROR, msg, (int)strlen(msg), lx->line };
    return t;
}

static void skip_whitespace(Lexer *lx) {
    for(;;) {
        char c = peek(lx); 
        if (c == ' ' || c == '\t' || c == '\r') advance(lx); 
        else if (c == '\n') { lx->line++; advance(lx); }
        else return;
    }
}

static Token number(Lexer *lx) {
    while (isdigit((unsigned char)peek(lx))) advance(lx);
    return make_token(lx, TOK_NUMBER);
}

static Token identifier(Lexer *lx) {
    while (isalnum((unsigned char)peek(lx)) || peek(lx) == '_') advance(lx);

    int len = (int)(lx->current - lx->start);
    if (len == 3 && memcmp(lx->start, "let", 3) == 0)   return make_token(lx, TOK_LET);
    if (len == 5 && memcmp(lx->start, "print", 5) == 0) return make_token(lx, TOK_PRINT);
    return make_token(lx, TOK_IDENT);
}

void lexer_init(Lexer *lx, const char *source) {
    // ok but where does this source then come from ?
    lx->start = source;
    lx->current = source; 
    lx->line = 1;
}

// it matters where this ends up getting called
Token next_token(Lexer *lx) {
    skip_whitespace(lx); 
    lx->start = lx->current;

    if(is_at_end(lx)) return make_token(lx, TOK_EOF);
    char c = advance(lx);

    if(isalpha((unsigned char)c) || c == '_') return identifier(lx);
    if(isdigit(c)) return number(lx);

    switch (c) {
        case '+': return make_token(lx, TOK_PLUS);
        case '-': return make_token(lx, TOK_MINUS);
        case '*': return make_token(lx, TOK_STAR);
        case '/': return make_token(lx, TOK_SLASH);
        case '(': return make_token(lx, TOK_LPAREN);
        case ')': return make_token(lx, TOK_RPAREN);
        case '=': return make_token(lx, TOK_EQUAL);
        case ';': return make_token(lx, TOK_SEMI);
    }

    return error_token(lx, "Unexpected character"); 

}



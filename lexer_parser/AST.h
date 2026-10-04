typedef enum {   
    NODE_NUM, 
    NODE_VAR,
    NODE_NEG, 
    NODE_BINARY 
} NodeKind;

typedef struct Node {
    NodeKind kind;
    int value;                  // NODE_NUM
    Token name;                 // NODE_VAR
    char op;                    // NODE_BINARY: '+', '-', '*', '/'
    struct Node *left, *right;  // NODE_BINARY uses both, NODE_NEG uses left
} Node;


typedef struct {
    Lexer lx;
    Token current;   // the next token, not yet consumed (the "lookahead")
    Token previous;  // the token we just consumed
} Parser;


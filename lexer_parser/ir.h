#ifndef IR_H
#define IR_H 

#include <stdio.h> 
#include "AST.h"


typedef enum { OPND_NONE, OPND_TEMP, OPND_CONST } OperandKind;

typedef struct {
    OperandKind kind; 
    long val; 
} Operand;

typedef enum {
    IR_COPY,    /* dst = a         */
    IR_NEG,     /* dst = -a        */
    IR_ADD,     /* dst = a + b     */
    IR_SUB,     /* dst = a - b     */
    IR_MUL,     /* dst = a * b     */
    IR_SDIV,    /* dst = a / b     (signed) */
    IR_SMOD,    /* dst = a % b     (signed) */
    IR_RET,     /* return a        */
} IrOp;

typedef struct Instr {
    IrOp op;
    Operand dst, a, b;
    struct Instr *next;
} Instr;

typedef struct {
    Instr *head, *tail;
    int ntemps;
} IrFunc;
 
/* AST -> IR. */
IrFunc *ir_from_expr(Node *expr);
 
/* Print IR as readable text, for debugging. */
void ir_print(IrFunc *f, FILE *out);
 
/* IR -> x86-64 assembly text. */
void emit_x86(IrFunc *f, FILE *out);
 
#endif

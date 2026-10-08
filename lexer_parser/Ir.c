#include <stdlib.h>
#include "ir.h"
 
/* ---------- building operands and instructions ---------- */
 
static Operand new_temp(IrFunc *f) {
    return (Operand){ OPND_TEMP, f->ntemps++ };
}
 
static Operand constant(long v) {
    return (Operand){ OPND_CONST, v };
}
 
static void emit(IrFunc *f, IrOp op, Operand dst, Operand a, Operand b) {
    Instr *i = calloc(1, sizeof *i);
    i->op = op; i->dst = dst; i->a = a; i->b = b;
    if (f->tail) f->tail->next = i; else f->head = i;
    f->tail = i;
}
 
static const Operand NONE = { OPND_NONE, 0 };
 
/* ---------- AST -> IR ----------
 * The core idea: gen_expr emits the instructions for a node and
 * RETURNS the operand that holds that node's value. Children are
 * generated first, so the order comes out right automatically.
 */
static Operand gen_expr(IrFunc *f, Node *n) {
    switch (n->kind) {
    case NODE_NUM:
        return constant(n->value);          /* no instruction needed */
 
    case NODE_NEG: {
        Operand a = gen_expr(f, n->binary.left);
        Operand t = new_temp(f);
        emit(f, IR_NEG, t, a, NONE);
        return t;
    }
 
    case NODE_BINARY: {
        Operand a = gen_expr(f, n->binary.left);
        Operand b = gen_expr(f, n->binary.right);
        IrOp op;
        switch (n->binary.op) {
        case '+': op = IR_ADD;  break;
        case '-': op = IR_SUB;  break;
        case '*': op = IR_MUL;  break;
        case '/': op = IR_SDIV; break;
        case '%': op = IR_SMOD; break;
        default:
            fprintf(stderr, "ir: unknown operator '%c'\n", n->binary.op);
            exit(1);
        }
        Operand t = new_temp(f);
        emit(f, op, t, a, b);
        return t;
    }
 
    case NODE_VAR:
        fprintf(stderr, "ir: variables not supported yet ('%.*s')\n",
                n->name.length, n->name.start);
        exit(1);
    }
    fprintf(stderr, "ir: unknown node kind %d\n", n->kind);
    exit(1);
}
 
IrFunc *ir_from_expr(Node *expr) {
    IrFunc *f = calloc(1, sizeof *f);
    Operand result = gen_expr(f, expr);
    emit(f, IR_RET, NONE, result, NONE);
    return f;
}
 
/* ---------- printing (debug only) ---------- */
 
static void print_operand(Operand o, FILE *out) {
    if (o.kind == OPND_TEMP)  fprintf(out, "t%ld", o.val);
    if (o.kind == OPND_CONST) fprintf(out, "%ld", o.val);
}
 
static const char *op_name[] = {
    [IR_COPY] = "copy", [IR_NEG] = "neg",  [IR_ADD]  = "add",
    [IR_SUB]  = "sub",  [IR_MUL] = "mul",  [IR_SDIV] = "sdiv",
    [IR_SMOD] = "smod", [IR_RET] = "ret",
};
 
void ir_print(IrFunc *f, FILE *out) {
    for (Instr *i = f->head; i; i = i->next) {
        fprintf(out, "    ");
        if (i->dst.kind != OPND_NONE) {
            print_operand(i->dst, out);
            fprintf(out, " = ");
        }
        fprintf(out, "%s ", op_name[i->op]);
        print_operand(i->a, out);
        if (i->b.kind != OPND_NONE) {
            fprintf(out, ", ");
            print_operand(i->b, out);
        }
        fprintf(out, "\n");
    }
}
#include "ir.h"
 
/* macOS prefixes C symbols with '_' (main -> _main); Linux doesn't. */
#ifdef __APPLE__
#define SYM "_"
#else
#define SYM ""
#endif
 
/* Load an operand into a register, e.g. load(o, "%rax", out). */
static void load(Operand o, const char *reg, FILE *out) {
    if (o.kind == OPND_TEMP)
        fprintf(out, "    movq    -%ld(%%rbp), %s\n", (o.val + 1) * 8, reg);
    else /* constant: movabsq handles any 64-bit value */
        fprintf(out, "    movabsq $%ld, %s\n", o.val, reg);
}
 
/* Store a register into a temp's stack slot. */
static void store(const char *reg, Operand dst, FILE *out) {
    fprintf(out, "    movq    %s, -%ld(%%rbp)\n", reg, (dst.val + 1) * 8);
}
 
void emit_x86(IrFunc *f, FILE *out) {
    /* Stack frame: 8 bytes per temp, rounded up to 16 so the stack
     * stays aligned (required before any call, e.g. to printf later). */
    int frame = ((f->ntemps * 8) + 15) & ~15;
 
    fprintf(out, "    .text\n");
    fprintf(out, "    .globl  " SYM "main\n");
    fprintf(out, SYM "main:\n");
    fprintf(out, "    pushq   %%rbp\n");          /* save caller's frame pointer */
    fprintf(out, "    movq    %%rsp, %%rbp\n");   /* our frame starts here */
    if (frame)
        fprintf(out, "    subq    $%d, %%rsp\n", frame);
 
    for (Instr *i = f->head; i; i = i->next) {
        switch (i->op) {
        case IR_COPY:
            load(i->a, "%rax", out);
            store("%rax", i->dst, out);
            break;
 
        case IR_NEG:
            load(i->a, "%rax", out);
            fprintf(out, "    negq    %%rax\n");
            store("%rax", i->dst, out);
            break;
 
        case IR_ADD:
        case IR_SUB:
        case IR_MUL: {
            const char *insn = i->op == IR_ADD ? "addq"
                             : i->op == IR_SUB ? "subq" : "imulq";
            load(i->a, "%rax", out);
            load(i->b, "%rcx", out);
            fprintf(out, "    %-7s %%rcx, %%rax\n", insn);   /* rax = rax OP rcx */
            store("%rax", i->dst, out);
            break;
        }
 
        case IR_SDIV:
        case IR_SMOD:
            /* idiv divides the 128-bit value rdx:rax by its operand.
             * cqo sign-extends rax into rdx first.
             * Quotient lands in rax, remainder in rdx. */
            load(i->a, "%rax", out);
            load(i->b, "%rcx", out);
            fprintf(out, "    cqo\n");
            fprintf(out, "    idivq   %%rcx\n");
            store(i->op == IR_SDIV ? "%rax" : "%rdx", i->dst, out);
            break;
 
        case IR_RET:
            load(i->a, "%rax", out);            /* return value goes in rax */
            fprintf(out, "    movq    %%rbp, %%rsp\n");
            fprintf(out, "    popq    %%rbp\n");
            fprintf(out, "    ret\n");
            break;
        }
    }
 
#ifndef __APPLE__
    /* Linux only: tell the linker this code doesn't need an executable stack. */
    fprintf(out, "    .section .note.GNU-stack,\"\",@progbits\n");
#endif
}
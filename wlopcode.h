#ifndef wl_opcode_h
#define wl_opcode_h


/*
 * Execution model:
 *   SECD machine
 *
 * Stacks:
 *   S(tack): value stack
 *   E(nvironment): binding stack
 *   C(ode): opcode stack
 *   D(ump): can store S/E/C value or write back
 *
 * How to run program:
 *   Pop opcode from C and execute opcode, while C is not empty.
 */  


typedef enum {
/*-----------------------------------------------------------------------*/
/* name       args  description                                          */
/*-----------------------------------------------------------------------*/
OP_LOAD,/*    A     S: (stk...) -> (A (stk...))                          */
OP_LOADI,/*   A     S: (stk...) -> (A (stk...))                          */
OP_LOADCH,/*  A     S: (stk...) -> (A (stk...))                          */
OP_LOADNIL,/*       S: (stk...) -> (nil (stk...))                        */
OP_LOADT,/*         S: (stk...) -> (t (stk...))                          */
/*                                                                       */
OP_CONS,/*          S: (A (B (stk...))) -> ((A . B) (stk...))            */
OP_CAR,/*           S: ((A . B) (stk...)) -> (A (stk...))                */
OP_CDR,/*           S: ((A . B) (stk...)) -> (B (stk...))                */
OP_EQ,/*            S: (A (B (stk...))) -> (t/nil (stk...))              */
OP_ATOM,/*          S: (A (stk...)) -> (t/nil (stk...))                  */
/*                                                                       */
OP_IF,/*            S: (A (stk...)) -> (stk...)                          */
/*                  C: [C1, (C2 (C3 (C4...)))] -> [C2, (C3 (C4...))]     */
/*                                             -> [C3, (C4...)]          */
} WlOpcode;

#endif

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
OP_LOAD,/*    A     S: (stop...) -> (A (stop...))                        */
OP_LOADNIL,/*       S: (stop...) -> (nil (stop...))                      */
OP_LOADT,/*         S: (stop...) -> (t (stop...))                        */
/*                                                                       */
OP_CONS,/*          S: (A (B (stop...))) -> ((A . B) (stop...))          */
OP_CAR,/*           S: ((A . B) (stk...)) -> (A (stk...))                */
OP_CDR,/*           S: ((A . B) (stk...)) -> (B (stk...))                */
OP_EQ,/*            S: (A (B (stk...))) -> (t/nil (stk...))              */
OP_ATOM,/*          S: (A (stk...)) -> (t/nil (stk...))                  */
/*                                                                       */
OP_IF,/*            S: (A (stk...)) -> (stk...)                          */
/*                  C: [C1, (C2 (C3 (C4...)))] -> [C2, (C3 (C4...))]     */
/*                                             -> [C3, (C4...)]          */
}

#endif

#include<stdio.h>
#include"wl_kernel.h"

int main(void) {
  WlCell op1, op2;
  WlKernelVM* vm;
  op1.type = WL_TYPE_WORD;
  op1.word_type = WL_WORD_DOTS;
  op2.type = WL_TYPE_INT;
  op2.num = 42;
  WlCell* program[] = {&op2, &op1};

  vm = wl_init_kernel(program, sizeof(program)/sizeof(WlCell*));
  wl_run_kernel(vm);
  return 0;
}

#include<stdio.h>
#include"wl_kernel.h"

void print_cell(const WlCell* c) {
  if (c == NULL) {
    printf("NULL");
    return;
  }
  switch (c->type) {
    case WL_TYPE_INT:
      printf("%d", c->num);
      break;
    case WL_TYPE_ARRAY:
      printf("array not implemented");
      break;
    case WL_TYPE_SYMBOL:
      printf("%s", c->name);
      break;
    case WL_TYPE_CONS:
      printf("cons not implemented");
      break;
    default:
      printf("unknown object");
  }
}

void print_stack(const WlStack* s) {
  printf("[");
  for (int i=0; i<=s->top; i++) {
    print_cell(s->stack[i]);
    if (i != s->top) {
      printf(" ");
    }
  }
  printf("]");
}

WlStack* wl_init_stack(int size) {
  WlStack* stack = (WlStack*)malloc(sizeof(WlStack));
  stack->size = size;
  stack->top = -1;
  stack->stack = (WlCell**)malloc(sizeof(WlCell) * size);
  return stack;
}

int wl_stack_full_p(WlStack* s) {
  return ((s->top + 1) >= s->size) ? WL_FALSE : WL_TRUE;
}

void wl_stack_resize(WlStack* s, int new_size) {
  WlCell** new_stack = (WlCell**)malloc(sizeof(WlCell) * new_size);
  for (int i=0; i<=s->top; i++) {
    new_stack[i] = s->stack[i];
  }
  free(s->stack);
  s->size = new_size;
  s->stack = new_stack;
}

void wl_stack_push(WlStack* s, WlCell* o) {
  if (wl_stack_full_p(s)) {
    wl_stack_resize(s, s->size + 1000);
  }
  s->top++;
  s->stack[s->top] = o;
}

WlCell* wl_stack_pop(WlStack* s) {
  if (s->top == -1) {
    return NULL;
  }
  return s->stack[s->top--];
}

WlCell* wl_stack_peek(WlStack* s) {
  if (s->top == -1) {
    return NULL;
  }
  return s->stack[s->top];
}

void wl_word_next(WlKernelVM* vm) {
  if (vm->ip->pos >= vm->ip->len) {
    free(vm->ip);
    vm->ip = NULL;
  } else {
    vm->ip->pos++;
  }
}

void wl_word_dots(WlKernelVM* vm) {
  printf(".s called!\n");
  print_stack(vm->dstack); printf("\n");
  wl_word_next(vm);
}

WORD_FN wl_words[] = {
  wl_word_next,
  wl_word_dots
};

WlKernelVM* wl_init_kernel(WlCell** program, int len) {
  WlKernelVM* vm = (WlKernelVM*)malloc(sizeof(WlKernelVM));
  WlCell* ip = (WlCell*)malloc(sizeof(WlCell));
  ip->pos = 0;
  ip->len = len;
  ip->program = program;
  vm->ip = ip;
  vm->dstack = wl_init_stack(100);
  vm->rstack = wl_init_stack(100);
  vm->cstack = wl_init_stack(100);
  return vm;
}

void wl_run_kernel(WlKernelVM* vm) {
  while (vm->ip != NULL) {
    WlCell* word = vm->ip->program[vm->ip->pos];
    switch (word->type) {
      case WL_TYPE_INT:
      case WL_TYPE_ARRAY:
      case WL_TYPE_SYMBOL:
      case WL_TYPE_CONS:
        wl_stack_push(vm->dstack, word);
        wl_word_next(vm);
        break;
      case WL_TYPE_PROC:
      case WL_TYPE_INSTPTR:
        printf("not implemented!\n");
        break;
      case WL_TYPE_WORD:
        wl_words[word->word_type](vm);
        break;
      default:
        printf("it may be an error\n");
    }
  }
  printf("end!!\n");
}

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

// * kernel: a virtual machine for wl
//
//   - threaded code
//   - stack based
//
#ifndef wl_kernel_h
#define wl_kernel_h

#include<stdlib.h>

#define WL_TRUE 1
#define WL_FALSE 0

// wl object
typedef enum {
  WL_TYPE_INT,
  WL_TYPE_ARRAY,
  WL_TYPE_SYMBOL,
  WL_TYPE_CONS,
  WL_TYPE_PROC
} WlType;

typedef struct WlObject {
  WlType type;

  union {
    int num;        // integer
    struct {        // array
      WlType a_type;
      int a_size;
      struct WlObject* array;
    };
    char* name;     // symbol
    struct {        // cons
      struct WlObject* car;
      struct WlObject* cdr;
    };
    struct {        // proc
      int p_size;
      struct WlObject** program;
    };
  };
} WlObject;

// stack
typedef struct WlStack {
  int top;
  int size;
  WlObject** stack;
} WlStack;

WlStack* wl_init_stack(int size) {
  WlStack* stack = (WlStack*)malloc(sizeof(WlStack));
  stack->size = size;
  stack->top = -1;
  stack->stack = (WlObject**)malloc(sizeof(WlObject) * size);
  return stack;
}

int wl_stack_full_p(WlStack* s) {
  return ((s->top + 1) >= s->size) ? WL_FALSE : WL_TRUE;
}

void wl_stack_resize(WlStack* s, int new_size) {
  WlObject** new_stack = (WlObject**)malloc(sizeof(WlObject) * new_size);
  for (int i=0; i<=s->top; i++) {
    new_stack[i] = s->stack[i];
  }
  free(s->stack);
  s->size = new_size;
  s->stack = new_stack;
}

void wl_stack_push(WlStack* s, WlObject* o) {
  if (wl_stack_full_p(s)) {
    wl_stack_resize(s, s->size + 1000);
  }
  s->top++;
  s->stack[s->top] = o;
}

WlObject* wl_stack_pop(WlStack* s) {
  if (s->top == -1) {
    return NULL;
  }
  return s->stack[s->top--];
}
WlObject* wl_stack_peek(WlStack* s) {
  if (s->top == -1) {
    return NULL;
  }
  return s->stack[s->top];
}

// VM
typedef struct WlKernelVM {
  unsigned int pc;
  WlObject* program;
  WlStack* dstack;
  WlStack* rstack;
  WlStack* cstack;
} WlKernelVM;

WlKernelVM* wl_init_kernel();

#endif

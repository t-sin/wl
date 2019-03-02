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
  WL_TYPE_INSTPTR,
  WL_TYPE_WORD,
  WL_TYPE_PROC
} WlType;

typedef enum {
  // VM primitives
  WL_WORD_NEXT,
  WL_WORD_DOTS,
  // word operations
  // integer operations
  // array operations
  // symbol operations
  // cons operations
  // proc operations
} WlWord;

typedef struct WlCell {
  WlType type;

  union {
    int num;           // integer
    struct {           // array
      WlType a_type;
      int a_size;
      struct WlCell* array;
    };
    char* name;        // symbol
    struct {           // cons
      struct WlCell* car;
      struct WlCell* cdr;
    };
    struct {           // instruction pointer
      int pos;
      int len;
      struct WlCell** program;
    };
    WlWord word_type;  // primitive word
    struct {           // user defined proc
      int p_size;
      struct WlCell** p_body;
    };
  };
} WlCell;

void print_cell(const WlCell* c);

typedef struct WlStack {
  int top;
  int size;
  WlCell** stack;
} WlStack;

void print_stack(const WlStack* s);
WlStack* wl_init_stack(int size);
int wl_stack_full_p(WlStack* s);
void wl_stack_resize(WlStack* s, int new_size);
void wl_stack_push(WlStack* s, WlCell* o);
WlCell* wl_stack_pop(WlStack* s);
WlCell* wl_stack_peek(WlStack* s);

typedef struct WlKernelVM {
  WlCell* ip;
  WlStack* dstack;
  WlStack* rstack;
  WlStack* cstack;
} WlKernelVM;

typedef void (*WORD_FN)(WlKernelVM*);
WlKernelVM* wl_init_kernel(WlCell** program, int len);
void wl_run_kernel(WlKernelVM* vm);

#endif

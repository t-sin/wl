// * kernel: a virtual machine for wl
//
//   - threaded code
//   - stack based
//
#ifdef wl_kernel_h
#define wl_kernel_h

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
      WlObject* car;
      WlObject* cdr;
    };
    struct {        // proc
      int p_size;
      struct WlObject** program;
    };
  }
} WlObject;

typedef struct WlStack {
  int top;
  WlObject** stack;
} WlStack;

int wl_stack_push(WlStack* s, const WlObject* o);
WlObject* wl_stack_pop(WlStack* s);
WlObject* wl_stack_peek(WlStack* s);
int wl_stack_full_p(WlStack* s);
void wl_stack_resize(WlStack* s);

typedef struct WlKernelVM {
  unsigned int pc;
  WlObject* program;
  WlStack* dstack;
  WlStack* rstack;
  WlStack* cstack;
} WlKernelVM;

WlKernelVM* wl_init_kernel();

#endif

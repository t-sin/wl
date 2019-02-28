// kernel: a virtual machine for wl
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
    // TODO: proc
  }
} WlObject;

typedef struct WlStack {
  int top;
  WlObject** stack;
} WlStack;

typedef struct WlKernelVM {
  unsigned int pc;
  int* program;
  WlStack* dstack;
} WlKernelVM;

#endif

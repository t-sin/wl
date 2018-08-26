#ifndef wl_object_h
#define wl_object_h

typedef enum {
  WL_TNIL
  WL_TCONS,
  WL_TSYMBOL,
  WL_TINT,
  WL_FUNCTION,
} WlType;

typedef struct WlCallStackFrame{
  struct WlCallStackFrame* parent;
  WlBinding* binding;
} WlCallStackFrame;

typedef struct WlObject {
  int id;
  enum WlType type;
  union {
    struct {
      struct WlObject* car;
      struct WlObject* cdr;
    };
    char** name;
    int num;
    struct {  // function (仮)
      WlType type;
      WlArgInfo arg;
      int** body;
    }
    struct {  // clozure (仮)
      WlType type;
      WlArgInfo arg;
      WlCallStackFrame* parent
      int** body;
    }
  }
} WlObject;

#endif

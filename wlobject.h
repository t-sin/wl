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

typedef union WlObject {
  struct {  // nil
    WlType type;
  } _nil;
  struct {  // cons
    WlType type;
    union WlObject* car;
    union WlObject* cdr;
  } _cons;
  struct {  // symbol
    WlType type;
    char** name;
  } _sym;
  struct {  // int
    WlType type;
    int num;
  } _int;
  struct {  // function
    WlType type;
    WlArgInfo arg;
    int** body;  // 仮
  } _fn;
  struct {  // clozure
    WlType type;
    WlArgInfo arg;
    WlCallStackFrame* parent
    int** body;  // 仮
  } _fnc;
} WlObject;

#endif
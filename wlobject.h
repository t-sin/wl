#ifndef wl_object_h
#define wl_object_h

typedef enum {
  WL_TINT,
  WL_TCHAR,
  WL_TARRAY,
  WL_TNIL,
  WL_TSYMBOL,
  WL_TCONS,
  WL_FUNCTION
} WlType;

typedef struct WlCallStackFrame{
  struct WlCallStackFrame* parent;
  WlBinding* binding;
} WlCallStackFrame;

typedef struct WlObject {
  int id;
  enum WlType type;
  union {
    int num;
    unsigned int cp;
    struct {
      enum WlType a_type;
      int a_size;
      union {
        struct Obj** array;
        int** ch_array;
    }
    char** name;
    struct {
      struct WlObject* car;
      struct WlObject* cdr;
    };
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

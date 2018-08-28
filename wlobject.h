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

// typedef struct WlCallStackFrame{
//   struct WlCallStackFrame* parent;
//   WlBinding* binding;
// } WlCallStackFrame;

typedef struct WlObject {
  int id;
  WlType type;

  union {
    int num;
    unsigned int cp;

    struct {
      WlType a_type;
      int a_size;
      union {
        struct WlObject** array;
        int** ch_array;
      };
    };

    char** name;

    struct {
      struct WlObject* car;
      struct WlObject* cdr;
    };

    struct {  // function (仮)
//      WlArgInfo arg;
      int** fn_body;
    };

    struct {  // clozure (仮)
//      WlArgInfo arg;
//      WlCallStackFrame* parent;
      int** cl_body;
    };
  };
} WlObject;

#endif

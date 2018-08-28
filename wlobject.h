#ifndef wl_object_h
#define wl_object_h

#include "wlhash.h"


typedef enum {
  WL_TINT,
  WL_TCHAR,
  WL_TARRAY,
  WL_TNIL,
  WL_TSYMBOL,
  WL_TCONS,
  WL_FUNCTION
} WlType;

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
      WlObject* args;
      WlEnvironment* enclosure;
      int** fn_body;
    };
  };
} WlObject;

typedef struct WlEnvironment {
  struct WlEnvironment* parent;
  WlObjectTable* var_table;
  WlObjectTable* func_table;
} WlEnvironment;

#endif

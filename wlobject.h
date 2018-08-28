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

    struct {
      WlObject* args;
      WlBinding* enclose;
      int** fn_body;
    };
  };
} WlObject;

WlObject* make_object(WlType type);
void free_object(WlObject* o);

#define IS_INT(o)    ((o)->type == WL_TINT)
#define IS_CHAR(o)   ((o)->type == WL_TCHAR)
#define IS_ARRAY(o)  ((o)->type == WL_TARRAY)
#define IS_NIL(o)    ((o)->type == WL_TNIL)
#define IS_SYM(o)    ((o)->type == WL_TSYMBOL)
#define IS_CONS(o)   ((o)->type == WL_TCONS)
#define IS_FUNC(o)   ((o)->type == WL_TFUNCTION)


typedef struct WlBinding {
  struct WlBinding* parent;
  WlObjectTable* table;
} WlBinding;

WlBinding* make_binding(WlBinding* parent);
void free_binding(WlBinding* b);


#endif

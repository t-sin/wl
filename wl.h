#ifndef wl_h
#define wl_h

enum WlType {
  WL_TNIL = 0,
  WL_TCONS,
  WL_TSYMBOL,
  WL_TINT,
  WL_FUNCTION
};

typedef struct {
  char *name;
} WlSymbol;

typedef struct WlObject {
  enum WlType type;
  struct WlObject* car;
  struct WlObject* cdr;
  WlSymbol* sym;
  int inum;
} WlObject;


int name_hash(const char* name);

typedef struct {
  int hash;
  WlSymbol* symbol;
} WlBindingKey;

typedef struct {
  WlBindingKey** key_table;
  WlObject** value_table;
  int item_count;
  int not_null_count;
  int d;  // hash_table.length = 2^d
} WlBinding;

WlBinding* make_binding();

WlSymbol* intern(WlBinding* table, WlSymbol* s);
WlBinding* unbind(WlBinding* b, WlSymbol* s);
WlBinding* find(WlBinding* b, const char* name);

#endif

#ifndef wl_h
#define wl_h

enum WlType {
  WL_TNIL = 0,
  WL_TCONS,
  WL_TSYMBOL,
  WL_TINT
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
  WlBindingKeyy** key_table;
  WlObject** value_table;
  int item_count;
  int not_null_count;
  int d;  // hash_table.length = 2^d
} WlBinding;

WlBinding* make_binding();

int add(WlBinding* table, WlSymbol* s);
WlBinding* remove(WlBinding* table, WlSymbol* s);
WlBinding* find(WlBinding* table, const char* name);

#endif

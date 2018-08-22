#ifndef wl_bind_h
#define wl_bind_h

#include "wl.h"


int hash(const char* name);

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

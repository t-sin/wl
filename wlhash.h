#ifndef wl_hashtable_h
#define wl_hashtable_h

#include "wlobject.h"

int wl_hash(int id);

typedef struct {
  int hash;
  int id;
} WlTableKey;

typedef struct {
  WlTableKey** keys;
  WlObject** values;

  int item_count;
  int not_null_count;
  int d;  // hash_table.length = 2^d
} WlObjectTable;

WlObjectTable* make_binding();

WlObject* table_add(WlObjectTable* table, WlObject* o, int resize);
WlObject* table_remove(WlObjectTable* table, WlObject* o);
WlObject* find(WlObjectTable* table, WlObject* o);

#endif

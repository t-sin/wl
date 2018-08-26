#ifndef wl_hashtable_h
#define wl_hashtable_h

#include "wlobject.h"

#define WL_DEFAULT_TABLE_SIZE 8

int hash(WlObject* o, int max_num);

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

WlObjectTable* make_table();

WlObject* table_add(WlObjectTable* table, WlObject* o, int resize);
WlObject* table_remove(WlObjectTable* table, WlObject* o);
WlObject* table_find(WlObjectTable* table, WlObject* o);

#endif

#ifndef wl_hashtable_h
#define wl_hashtable_h

#include "wlobject.h"

#define WL_DEFAULT_TABLE_SIZE 1

int hash_id(int id, int max_num);
int hash_obj(WlObject* o, int max_num);

enum WlTableState {
  WL_HASHTABLE_NULL,
  WL_HASHTABLE_DELETED,
  WL_HASHTABLE_FILLED
};

typedef struct {
  enum WlTableState state;
  int id;
} WlTableKey;

typedef struct {
  WlTableKey* keys;
  WlObject** values;

  int item_count;
  int not_null_count;
  int d;  // hash_table.length = 2^d
} WlObjectTable;

WlObjectTable* make_table();
WlObjectTable* table_resize(WlObjectTable* table);
void free_table(WlObjectTable* table);

WlObject* table_find(WlObjectTable* table, WlObject* k);
WlObject* table_add(WlObjectTable* table, WlObject* k, WlObject* v);
WlObject* table_remove(WlObjectTable* table, WlObject* k);

#endif

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <stdio.h>

#include "wlobject.h"
#include "wlhash.h"

int hash_id(int id, int max_num) {
  return id % max_num;
}

int hash_obj(WlObject* o, int max_num) {
  return hash_id(o->id, max_num);
}

#define table_size(t) ((int)pow(2, t->d))

WlObjectTable* make_table() {
  WlObjectTable* table;
  void* mem;
  
  mem = malloc(sizeof(WlObjectTable));
  if (mem == NULL) {
    return NULL;
  }
  table = (WlObjectTable*)mem;

  table->d = WL_DEFAULT_TABLE_SIZE;
  table->item_count = 0;
  table->not_null_count = 0;

  mem = malloc(sizeof(WlTableKey) * table_size(table));
  if (mem == NULL) {
    return NULL;
  }
  memset(mem, 0, sizeof(WlTableKey) * table_size(table));
  table->keys = (WlTableKey*)mem;

  mem = malloc(sizeof(WlObject*) * table_size(table));
  if (mem == NULL) {
    return NULL;
  }
  memset(mem, 0, sizeof(WlObject*) * table_size(table));
  table->values = (WlObject**)mem;

  return table;
}

int _table_set_key(int size, WlTableKey* keys, int key_id, enum WlTableState state) {
  int hash_code = hash_id(key_id, size);
  WlTableKey* key;

  for (int i=0; hash_code+i<size; i++) {
    key = keys + (hash_code + i);
    if (key->state == WL_HASHTABLE_NULL || key->state == WL_HASHTABLE_DELETED) {
      key->id = key_id;
      key->hash = hash_code;
      key->state = state;
      return hash_code + i;
    }
  }
  return -1;
}

WlObject* table_add(WlObjectTable* table, WlObject* k, WlObject* v) {
  int idx = _table_set_key(table_size(table), table->keys, k->id, WL_HASHTABLE_FILLED);
  if (idx == -1) {
    return NULL;
  } else {
    table->values[idx] = v;
    table->item_count++;
    return v;
  }
}

    return NULL;
  }
}

void table_print(WlObjectTable* table) {
  for (int i=0; i<table_size(table); i++) {
    WlTableKey key = table->keys[i];
    WlObject* val = table->values[i];
    if (key.state == WL_HASHTABLE_FILLED) {
      printf("%d: %d\n", key.id,  val->id);
    }
  }
}

int main(void) {
  WlObjectTable* table= make_table();
  WlObject key, val;
  key.id = 42;
  val.id = 100;

  table_add(table, &key, &val, 0);

  table_print(table);

  return 0;
}

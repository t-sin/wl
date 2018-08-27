#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <stdio.h>

#include "wlobject.h"
#include "wlhash.h"

int hash(WlObject* o, int max_num) {
  return o->id % max_num;
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

WlObject* table_add(WlObjectTable* table, WlObject* k, WlObject* v, enum WlTableFulfilled resize_p) {
  int hash_code = hash(k, table_size(table));
  WlTableKey* key;

  printf("hash code: %d\n", hash_code);
  for (int i=0; hash_code+i<table_size(table); i++) {
    key = table->keys + (hash_code + i);
    printf("  i: %d\n", i);

    if (key->state == WL_HASHTABLE_NULL || key->state == WL_HASHTABLE_DELETED) {
      key->id = k->id;
      key->hash = hash_code;
      key->state = WL_HASHTABLE_FILLED;
      table->values[hash_code] = v;
      (table->item_count)++;
      return v;
    }
  }

  if (resize_p == WL_HASHTABLE_ERROR) {
    return NULL;
  } else {
    // TODO: resize hashtable...
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

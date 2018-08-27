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

void free_table(WlObjectTable* table) {
  free(table->keys);
  free(table->values);
  free(table);
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

WlObjectTable* table_resize(WlObjectTable* table) {
  int new_size = (int)pow(2, table->d + 1);
  WlTableKey* keys = (WlTableKey*)malloc(sizeof(WlTableKey) * new_size);
  WlObject** values = (WlObject**)malloc(sizeof(WlObject) * new_size);
  WlTableKey* key;
  int hash_code, idx;

  if (keys == NULL || values == NULL) {
    return NULL;
  }

  for (int i=0; i<table_size(table); i++) {
    key = table->keys + i;
    if (key->state == WL_HASHTABLE_FILLED) {
      idx = _table_set_key(new_size, keys, key->id, WL_HASHTABLE_FILLED);
      if (idx == -1) {
        free(keys);
        free(values);
        return NULL;
      }
      values[idx] = table->values[i];
    }
  }
  free(table->keys);
  free(table->values);
  table->d++;
  table->keys = keys;
  table->values = values;
  return table;
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
  WlObject key2, val2;
  key.id = 42;
  val.id = 100;
  key2.id = 22;
  val2.id = 200;

  table_add(table, &key, &val);
  table_add(table, &key2, &val2);
  table_print(table);
  printf("d: %d\n", table->d);
  printf("length: %d\n\n", table->item_count);

  table_resize(table);
  table_print(table);
  printf("d: %d\n", table->d);
  printf("length: %d\n\n", table->item_count);

  free_table(table);

  return 0;
}

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "wlobject.h"
#include "wlhash.h"

int hash_id(int id, int max_num) {
  return id % max_num;
}

int hash_obj(WlObject* o, int max_num) {
  return hash_id(o->id, max_num);
}

#define table_size(t) ((1<<t->d))

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

WlObject* table_find(WlObjectTable* table, WlObject* k) {
  int i = hash_obj(k, table_size(table));
  WlTableKey* key = table->keys + i;
  while (key->state != WL_HASHTABLE_NULL) {
    if (key->state != WL_HASHTABLE_DELETED && key->id == k->id) {
      return table->values[i];
    }
    i = (i == table_size(table) - 1) ? 0 : i + 1;
    key = table->keys + i;
  }
  return NULL;
}

WlObject* table_add(WlObjectTable* table, WlObject* k, WlObject* v) {
  if (table_find(table, k) != NULL) return NULL;
  if (2 * (table->not_null_count + 1) > table_size(table)) table_resize(table);
  int i = hash_obj(k, table_size(table));
  WlTableKey* key = table->keys + i;

  while (key->state != WL_HASHTABLE_NULL && key->state != WL_HASHTABLE_DELETED) {
    i = (i == table_size(table) - 1) ? 0 : i + 1;
    key = table->keys + i;
  }
  if (key->state == WL_HASHTABLE_NULL) table->not_null_count++;
  table->item_count++;
  key->id = k->id;
  key->state = WL_HASHTABLE_FILLED;
  table->values[i] = v;
  return v;
}

WlObject* table_remove(WlObjectTable* table, WlObject* k) {
  int i = hash_obj(k, table_size(table));
  WlTableKey* key = table->keys + i;

  while (key->state != WL_HASHTABLE_NULL) {
    if (key->state != WL_HASHTABLE_DELETED && key->state != WL_HASHTABLE_NULL) {
      key->state = WL_HASHTABLE_DELETED;
      table->item_count--;
      if (8 * table->item_count < table_size(table)) table_resize(table);
      return table->values[i];
    }
    i = (i == table_size(table)) ? 0 : i + 1;
  }
  return NULL;
}

WlObjectTable* table_resize(WlObjectTable* table) {
  int new_d = 1;
  WlTableKey* key;
  WlTableKey* keys;
  WlObject** values;

  while ((1<<new_d) < 3 * table->item_count) new_d++;

  keys = (WlTableKey*)malloc(sizeof(WlTableKey) * (1<<new_d));
  values = (WlObject**)malloc(sizeof(WlObject) * (1<<new_d));

  for (int k=0; k<table_size(table); k++) {
    key = table->keys + k;
    if (key->state == WL_HASHTABLE_FILLED) {
      int i = hash_id(key->id, 1<<new_d);
      while ((keys + i)->state != WL_HASHTABLE_NULL)
        i = (i == (1<<new_d) - 1) ? 0 : i + 1;
      keys[i].state = WL_HASHTABLE_FILLED;
      keys[i].id = key->id;
      values[i] = table->values[k];
    }
  }
  free(table->keys);
  free(table->values);
  table->d = new_d;
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

  printf("table[42] = %d\n", table_find(table, &key)->id);
  printf("found\n\n");

  table_resize(table);
  puts("resized\n\n");

  table_print(table);
  printf("d: %d\n", table->d);
  printf("length: %d\n\n", table->item_count);

  table_remove(table, &key);
  table_print(table);
  printf("d: %d\n", table->d);
  printf("length: %d\n\n", table->item_count);

  free_table(table);

  return 0;
}

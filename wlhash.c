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
  table->keys = (WlTableKey**)mem;
  for (int i=0; i<table_size(table); i++) {
    mem = malloc(sizeof(WlTableKey));
    if (mem == NULL) { return NULL; }
    table->keys[i] = (WlTableKey*)mem;
  }

  mem = malloc(sizeof(WlObject) * table_size(table));
  if (mem == NULL) {
    return NULL;
  }
  memset(mem, 0, sizeof(WlObject) * table_size(table));
  table->values = (WlObject**)mem;

  return table;
}


int main(void) {
  WlObjectTable* table;
  printf("before init: %x\n", (int)table);
  table = make_table();
  printf("after init: %x\n", (int)table);

  return 0;
}
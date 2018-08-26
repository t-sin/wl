#include <stdlib.h>
#include <math.h>

#include <stdio.h>

#include "wlobject.h"
#include "wlhash.h"

int hash(WlObject* o, int max_num) {
  return o->id % max_num;
}

WlObjectTable* make_table() {
  WlObjectTable* table;
  void* mem;
  
  mem = malloc(sizeof(WlObjectTable));
  if (mem == NULL) {
    return NULL;
  }
  table = (WlObjectTable*)mem;

  table->d = WL_DEFALUT_TABLE_SIZE;
  table->item_count = 0;
  table->not_null_count = 0;

  mem = malloc(sizeof(WlTableKey) * pow(2.0, table->d));
  if (mem == NULL) {
    return NULL;
  }
  table->keys = (WlTableKey**)mem;

  mem = malloc(sizeof(WlObject) * pow(2.0, table->d));
  if (mem == NULL) {
    return NULL;
  }
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
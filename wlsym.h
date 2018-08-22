#ifndef WL_SYMBOL
#define WL_SYMBOL

#include "wl.h"


int hash(const char* name);

typedef struct {
  int hash;
  WlSymbol* symbol;
} WlSymTableEntry;

typedef struct {
  WlSymTableKey** key_table;
  WlObject** values;
  int item_count;
  int not_null_count;
  int d;  // hash_table.length = 2^d
} WlSymTable;

WlSymTable* make_symtable();

int add(WlSymTable* table, WlSymbol* s);
struct WlSymbol* remove(WlSymTable* table, WlSymbol* s);
struct WlSymbol* find(WlSymTable* table, const char* name);

#endif

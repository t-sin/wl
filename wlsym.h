#ifndef WL_SYMBOL
#define WL_SYMBOL

struct WlSymbol {
  char* name;
};
typedef WlSymbol tWlSymbol;

// string -> hash code
int hash(const char* name);

struct WlSymTable {
  // hash code -> index
  int* hash_table;
  // array of symbols
  struct WlSymbol** symbols;
  int item_count;
  int not_null_count;
  // hash_table.length = 2^d
  int d;
};
typedef WlSymTable tWlSymTable;

tWlSymTable* make_symtable();

int add(tWlSymTable* table, tWlSymbol* s);
struct WlSymbol* remove(tWlSymTable* table, tWlSymbol* s);
struct WlSymbol* find(tWlSymTable* table, const char* name);

#endif

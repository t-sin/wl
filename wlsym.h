#ifndef WL_SYMBOL
#define WL_SYMBOL

typedef struct {
  char* name;
} WlSymbol;

// string -> hash code
int hash(const char* name);

typedef struct {
  int hash;
  WlSymbol* symbol;
} WlSymTableEntry;

typedef struct {
  // hash code -> index
  WlSymTableEntry hash_table;
  // array of symbols
  struct WlSymbol** symbols;
  int item_count;
  int not_null_count;
  // hash_table.length = 2^d
  int d;
} WlSymTable;

WlSymTable* make_symtable();

int add(WlSymTable* table, WlSymbol* s);
struct WlSymbol* remove(WlSymTable* table, WlSymbol* s);
struct WlSymbol* find(WlSymTable* table, const char* name);

#endif

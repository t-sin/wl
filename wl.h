#ifndef WL
#define WL


enum WlType {
  WL_TNIL = 0,
  WL_TCONS,
  WL_TSYMBOL,
  WL_TINT
};

struct WlSymbol {
  char *name;
};

struct WlObject {
  enum WlType type;
  struct WlObject *car;
  struct WlObject *cdr;
  struct WlSymbol *sym;
  int inum;
};

#endif

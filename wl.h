#ifndef wl_h
#define wl_h

enum WlType {
  WL_TNIL = 0,
  WL_TCONS,
  WL_TSYMBOL,
  WL_TINT
};

typedef struct {
  char *name;
} WlSymbol;

typedef struct WlObject {
  enum WlType type;
  struct WlObject* car;
  struct WlObject* cdr;
  WlSymbol* sym;
  int inum;
} WlObject;

#endif

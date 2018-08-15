#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char* wl_print(struct WlObject *o) {
  char *result;
  size_t result_len;
  char *scar;
  char *scdr;
  const char *s_nil = "nil";

  switch (o->type) {

  case WL_TNIL:
    result_len = sizeof(&s_nil);
    result = (char *)malloc(result_len);
    strncpy(result, s_nil, result_len);
    return result;

  case WL_TCONS:
    scar = wl_print(o->car);
    scdr = wl_print(o->cdr);
    result_len = strlen(scar) + strlen(scdr) + 5 + 1;
    result = (char *)malloc(result_len);
    snprintf(result, result_len, "(%s . %s)", scar, scdr);
    free(scar);
    free(scdr);
    return result;

  case WL_TSYMBOL:
    printf("symbol=%s\n", o->sym->name);
    break;

  case WL_TINT:
    result_len = 256;
    result = (char *)malloc(result_len);
    snprintf(result, result_len, "%d", o->inum);
    return result;

  default:
    printf("error!!!!!\n");
  }
}

int main(void) {
  struct WlObject cons;
  struct WlObject car, cdr;
  car.type = WL_TNIL;
  cdr.type = WL_TNIL;
  cons.type = WL_TCONS;
  cons.car = &car;
  cons.cdr = &cdr;

  puts(wl_print(&cons));
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wl.h"


struct WlObject* wl_read(const char *input);

struct WlObject* wl_eval(struct WlObject *o);

/*
 * `wl_print()` returns char* that string representation of `o`.
 * The pointer returned must be `free()`.
 */
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
  struct WlObject cons, nil;
  struct WlObject car, cdr;
  nil.type = WL_TNIL;
  car.type = WL_TCONS;
  car.car = &nil;
  car.cdr = &nil;
  cdr.type = WL_TNIL;
  cons.type = WL_TCONS;
  cons.car = &car;
  cons.cdr = &cdr;

  puts(wl_print(&cons));
  return 0;
}

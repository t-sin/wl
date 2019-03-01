#include<stdio.h>
#include"wl_kernel.h"

void print_obj(const WlCell* o) {
  if (o == NULL) {
    printf("NULL");
    return;
  }
  switch (o->type) {
    case WL_TYPE_INT:
      printf("%d", o->num);
      break;
    case WL_TYPE_ARRAY:
      printf("array not implemented");
      break;
    case WL_TYPE_SYMBOL:
      printf("%s", o->name);
      break;
    case WL_TYPE_CONS:
      printf("cons not implemented");
      break;
    default:
      printf("unknown object");
  }
}

void print_stack(const WlStack* s) {
  printf("[");
  for (int i=0; i<=s->top; i++) {
    print_obj(s->stack[i]);
    if (i != s->top) {
      printf(" ");
    }
  }
  printf("]");
}

int main(void) {
  WlCell o1, o2;
  WlStack* s;
  o1.type = WL_TYPE_INT;
  o1.num = 1;
  o2.type = WL_TYPE_INT;
  o2.num = 2;

  s = wl_init_stack(10);
  print_stack(s); printf("\n");
  wl_stack_push(s, &o1);
  print_stack(s); printf("\n");
  wl_stack_push(s, &o2);
  printf("poped: "); print_obj(wl_stack_pop(s)); printf("\n");
  printf("peeked: "); print_obj(wl_stack_peek(s)); printf("\n");
  printf("poped: "); print_obj(wl_stack_pop(s)); printf("\n");
  print_stack(s); printf("\n");
  printf("poped: "); print_obj(wl_stack_pop(s)); printf("\n");
  printf("poped: "); print_obj(wl_stack_pop(s)); printf("\n");
  print_stack(s); printf("\n");
  printf("poped: "); print_obj(wl_stack_pop(s)); printf("\n");

  return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "wl_kernel.h"

int main() {
    struct WlToken* tokens[100];
    char input[] = "/test { 1 } { 1 { 2 } 3 } 2 3";
    printf("input: '%s'\n", input);
    struct WlStream stream = {0, sizeof(input), input};

    int i = 0;
    struct WlToken* t;
    do {
        t = wl_parse_one(&stream);
        tokens[i++] = t;
    } while (t != NULL);

    struct WlVm* vm = wl_init_vm();
    vm->program = wl_compile(vm, tokens);
    wl_eval(vm);
    wl_eval(vm);
    wl_eval(vm);

    print_stack(vm->dstack);
}

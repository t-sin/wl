#include <stdio.h>
#include <stdlib.h>

#include "wl_kernel.h"

int main() {
    struct WlToken* tokens[100];
    char input[] = "/test /testt { 1 { 42 } 2 } 2 3";
    printf("input: '%s'\n", input);
    struct WlStream stream = {0, sizeof(input), input};

    int i = 0;
    struct WlToken* t;
    do {
        t = wl_parse_one(&stream);
        tokens[i++] = t;
    } while (t != NULL);

    struct WlVm* vm = wl_init_vm();
    int ip = 0;
    vm->program = wl_compile(vm, tokens, &ip);
    wl_eval(vm);

    for (i=0; vm->program[i] != NULL; i++) {
        print_cell(vm->program[i]);
        printf("\n");
    }
    print_stack(vm->dstack);
}

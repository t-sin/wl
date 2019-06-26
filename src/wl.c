#include <stdio.h>
#include <stdlib.h>

#include "wl_kernel.h"

int main() {
    struct WlToken* tokens[100];
    struct WlStream stream = {0, 30, "/test { 1 2 3 } def"};

    int i = 0;
    struct WlToken* t;
    do {
        t = wl_parse_one(&stream);
        tokens[i++] = t;
    } while (t != NULL);

    for (i=0; tokens[i] != NULL; i++) {
        print_token(tokens[i]);
        printf(" ");
    }
    printf("\n");

    // struct WlCell c = { WL_CELL_TYPE_INT, 42 };
    // struct WlCell c2 = { WL_CELL_TYPE_CHAR, 42 };

    // struct WlVm* vm = wl_init_vm();
}

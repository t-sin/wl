#include <stdio.h>

#include "wl_kernel.h"

void print_cell(const struct WlCell* c) {
    switch (c->type) {
    case WL_CELL_TYPE_INT:
        printf("<int: %d>", c->u.num);
        break;
    case WL_CELL_TYPE_CHAR:
        printf("<ch: %c>", c->u.ch);
        break;
    case WL_CELL_TYPE_STR:
        printf("<str: %s>", c->u.str);
        break;
    case WL_CELL_TYPE_PROC:
        printf("<proc: %d>", c->u.num);
        break;
    }
}

int main() {
    struct WlCell c = { WL_CELL_TYPE_INT , 42 };
    print_cell(&c); puts("");
}
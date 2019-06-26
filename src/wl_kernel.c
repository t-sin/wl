#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void print_stack(const struct WlStack* s) {
    printf("[");
    for (int i=0; i<=s->top; i++) {
        print_cell(s->stack[i]);
        if (i != s->top) {
            printf(" ");
        }
    }
    printf("]");
}

struct WlStack* wl_init_stack(uint32_t size) {
    struct WlStack* stack = (struct WlStack*)malloc(sizeof(struct WlStack));
    stack->size = size;
    stack->top = -1;
    stack->stack = (struct WlCell**)malloc(sizeof(struct WlCell) * size);
    return stack;
}

bool wl_stack_full_p(struct WlStack* s) {
    return ((s->top + 1) >= s->size) ? false : true;
}

void wl_stack_resize(struct WlStack* s, uint32_t new_size) {
    struct WlCell** new_stack = (struct WlCell**)malloc(sizeof(struct WlCell) * new_size);
    for (int i=0; i<=s->top; i++) {
        new_stack[i] = s->stack[i];
    }
    free(s->stack);
    s->size = new_size;
    s->stack = new_stack;
}

void wl_stack_push(struct WlStack* s, struct WlCell* o) {
    if (wl_stack_full_p(s)) {
        wl_stack_resize(s, s->size + 1000);
    }
    s->top++;
    s->stack[s->top] = o;
}

struct WlCell* wl_stack_pop(struct WlStack* s) {
    if (s->top == -1) {
        return NULL;
    }
    return s->stack[s->top--];
}

struct WlCell* wl_stack_peek(struct WlStack* s) {
    if (s->top == -1) {
        return NULL;
    }
    return s->stack[s->top];
}

void print_dict(const struct WlDict* dict) {
    const struct WlDict* d = dict;
    while (d != NULL) {
        printf(" * %s, ", d->name);
        print_cell(&d->data);
        printf(", %d\n", d->bytecode);
        d = d->next;
    }
}

struct WlDict* dict_find(struct WlDict* dict, char* name) {
    struct WlDict* d = dict;
    while (d != NULL) {
        if (strcmp(d->name, name) == 0) {
            return d;
        }
        d = d->next;
    }
    return NULL;
}

int main() {
    struct WlCell c = { WL_CELL_TYPE_INT, 42 };
    struct WlCell c2 = { WL_CELL_TYPE_CHAR, 42 };
    struct WlStack* stack = wl_init_stack(10);

    struct WlDict d1 = { "name", NULL, c, NULL };
    struct WlDict d2 = { "top", &d1, c2, NULL };

    print_dict(dict_find(&d2, "name"));
}
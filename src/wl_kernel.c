#include <ctype.h>
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
    case WL_CELL_TYPE_NAME:
        printf("<name: %s>", c->u.name);
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
        printf(", %d\n", d->code);
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

int wl_stream_peek(struct WlStream* s) {
    if (s->idx < s->len) {
        return s->buf[s->idx];
    } else {
        return EOF;
    }
}

int wl_stream_read(struct WlStream* s) {
    if (s->idx < s->len) {
        return s->buf[s->idx++];
    } else {
        return EOF;
    }
}

struct WlVm* wl_init_vm() {
    struct WlVm* vm = (struct WlVm*)malloc(sizeof(struct WlVm));
    vm->dict = NULL;
    vm->dstack = wl_init_stack(1024);
    vm->rstack = wl_init_stack(1024);
    vm_create_dict(vm);
    return vm;
}

void vm_create_dict(struct WlVm* vm) {
    struct WlDict* dict = (struct WlDict*)malloc(sizeof(struct WlDict));
    dict->name = NULL;
    dict->next = vm->dict;
    dict->code = NULL;
    vm->dict = dict;
}

struct WlToken* wl_parse_one(struct WlStream* s) {
    int ch = wl_stream_peek(s);
    int idx = 0;
    char* name;

    if (ch == ' ') {
        wl_stream_read(s);
        while ((ch = wl_stream_peek(s)) == ' ') {
            wl_stream_read(s);
        }
    }

    if (ch == EOF) {
        return NULL;

    } else if (ch == '{') {
        wl_stream_read(s);
        struct WlToken* token = (struct WlToken*)malloc(sizeof(struct WlToken));
        token->type = WL_TOKEN_OPEN_CURLY;
        return token;

    } else if (ch == '}') {
        wl_stream_read(s);
        struct WlToken* token = (struct WlToken*)malloc(sizeof(struct WlToken));
        token->type = WL_TOKEN_CLOSE_CURLY;
        return token;

    } else if (ch == '/') {
        wl_stream_read(s);
        name = (char*)malloc(sizeof(char) * NAME_LENGTH);
        memset(name, 0, sizeof(char) * NAME_LENGTH);
        while (isalpha(ch = wl_stream_peek(s))) {
            name[idx++] = wl_stream_read(s);
        }
        struct WlToken* token = (struct WlToken*)malloc(sizeof(struct WlToken));
        token->type = WL_TOKEN_LIT_NAME;
        token->u.str = name;
        return token;

    } else if (isalpha(ch)) {
        name = (char*)malloc(sizeof(char) * NAME_LENGTH);
        memset(name, 0, sizeof(char) * NAME_LENGTH);
        do {
            name[idx++] = wl_stream_read(s);
        } while (isalpha(ch = wl_stream_peek(s)));
        struct WlToken* token = (struct WlToken*)malloc(sizeof(struct WlToken));
        token->type = WL_TOKEN_NAME;
        token->u.str = name;
        return token;

    } else if (isdigit(ch)) {
        name = (char*)malloc(sizeof(char) * NAME_LENGTH);
        memset(name, 0, sizeof(char) * NAME_LENGTH);
        do {
            name[idx++] = wl_stream_read(s);
        } while (isdigit(ch = wl_stream_peek(s)));
        struct WlToken* token = (struct WlToken*)malloc(sizeof(struct WlToken));
        token->type = WL_TOKEN_NUMBER;
        token->u.num = atoi(name);
        return token;

    } else {
        return NULL;
    }
}

void print_token(struct WlToken* token) {
    switch (token->type) {
    case WL_TOKEN_NUMBER:
        printf("%d", token->u.num); break;
    case WL_TOKEN_CHAR:
        printf("'%c'", token->u.ch); break;
    case WL_TOKEN_NAME:
        printf("%s", token->u.name); break;
    case WL_TOKEN_LIT_NAME:
        printf("/%s", token->u.name); break;
    case WL_TOKEN_OPEN_CURLY:
        printf("{"); break;
    case WL_TOKEN_CLOSE_CURLY:
        printf("}"); break;
    case WL_TOKEN_NONE:
        ;
    }
}

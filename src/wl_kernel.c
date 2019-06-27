#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wl_kernel.h"

void print_cell(const struct WlCell* c) {
    switch (c->type) {
    case WL_CELL_INT:
        printf("<int:%d>", c->u.num);
        break;
    case WL_CELL_CHAR:
        printf("<ch:%c>", c->u.ch);
        break;
    case WL_CELL_STR:
        printf("<str:%s>", c->u.str);
        break;
    case WL_CELL_NAME:
        printf("<name:%s>", c->u.name);
        break;
    case WL_CELL_BUILTIN:
        printf("<builtin:%ld>", (long int)c->u.builtin);
        break;
    case WL_CELL_PROC:
        printf("<proc:%ld>", (long int)c->u.code);
        break;
    case WL_CELL_IP:
        printf("<ip:%d>", c->u.ip->pos);
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
        //printf(", %d\n", d->code);
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

void vm_op_pstack(struct WlVm* vm) {
    print_stack(vm->dstack);
    printf("\n");
}

void wl_init_dict(struct WlVm* vm) {
    // print stack
    vm_create_dict(vm);
    vm->dict->name = ".s";
    vm->dict->code = (struct WlCell*)malloc(sizeof(struct WlCell));
    vm->dict->code->type = WL_CELL_BUILTIN;
    vm->dict->code->u.builtin = vm_op_pstack;
};

struct WlVm* wl_init_vm() {
    struct WlVm* vm = (struct WlVm*)malloc(sizeof(struct WlVm));
    vm->ip = (struct WlCell*)malloc(sizeof(struct WlCell));
    vm->ip->type = WL_CELL_IP;
    vm->ip->u.ip = (struct WlIp*)malloc(sizeof(struct WlIp));
    vm->ip->u.ip->pos = 0;
    vm->ip->u.ip->code = NULL;
    vm->dict = NULL;
    vm->dstack = wl_init_stack(1024);
    vm->rstack = wl_init_stack(1024);
    vm_create_dict(vm);
    vm->dict->name = "[dict-bottom]";
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
        while (isalnum(ch = wl_stream_peek(s))) {
            name[idx++] = wl_stream_read(s);
        }
        struct WlToken* token = (struct WlToken*)malloc(sizeof(struct WlToken));
        token->type = WL_TOKEN_LIT_NAME;
        token->u.str = name;
        return token;

    } else if (isdigit(ch)) {
        name = (char*)malloc(sizeof(char) * NAME_LENGTH);
        memset(name, 0, sizeof(char) * NAME_LENGTH);
        while (isdigit(ch = wl_stream_peek(s))) {
            name[idx++] = wl_stream_read(s);
        }
        struct WlToken* token = (struct WlToken*)malloc(sizeof(struct WlToken));
        token->type = WL_TOKEN_NUMBER;
        token->u.num = atoi(name);
        return token;

    } else if (isgraph(ch)) {
        name = (char*)malloc(sizeof(char) * NAME_LENGTH);
        memset(name, 0, sizeof(char) * NAME_LENGTH);
        while (isgraph(ch = wl_stream_peek(s))) {
            name[idx++] = wl_stream_read(s);
        }
        struct WlToken* token = (struct WlToken*)malloc(sizeof(struct WlToken));
        token->type = WL_TOKEN_NAME;
        token->u.str = name;
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

struct WlCell** wl_compile(struct WlVm* vm, struct WlToken** tokens) {
    struct WlStack* cstack = wl_init_stack(1024);
    struct WlCell* ip = (struct WlCell*)malloc(sizeof(struct WlCell));
    ip->type = WL_CELL_IP;
    ip->u.ip = (struct WlIp*)malloc(sizeof(struct WlIp));
    ip->u.ip->pos = 0;
    ip->u.ip->code = (struct WlCell**)malloc(sizeof(struct WlCell*) * 1024);
    wl_stack_push(cstack, ip);

    struct WlCell* c, *ip_tmp;
    struct WlDict* dict;

    for (int i=0; tokens[i] != NULL; i++) {
        struct WlToken* t = tokens[i];
        switch (t->type) {
        case WL_TOKEN_NUMBER:
            c = (struct WlCell*)malloc(sizeof(struct WlCell));
            c->type = WL_CELL_INT;
            c->u.num = t->u.num;
            ip_tmp = wl_stack_peek(cstack);
            ip_tmp->u.ip->code[ip_tmp->u.ip->pos++] = c;
            break;

        case WL_TOKEN_CHAR:
            c = (struct WlCell*)malloc(sizeof(struct WlCell));
            c->type = WL_CELL_CHAR;
            c->u.num = t->u.ch;
            ip_tmp = wl_stack_peek(cstack);
            ip_tmp->u.ip->code[ip_tmp->u.ip->pos++] = c;
            break;

        case WL_TOKEN_NAME:
            ip_tmp = wl_stack_peek(cstack);
            dict = dict_find(vm->dict, t->u.name);
            if (dict == NULL) {
                printf("proc '%s' is not found!\n", t->u.name);
                return ip_tmp->u.ip->code;
            }
            c = (struct WlCell*)malloc(sizeof(struct WlCell));
            if (dict->code->type == WL_CELL_PROC) {
                c->type = WL_CELL_PROC;
                c->u.code = dict->code->u.code;
            } else { // WL_CELL_BULTIN
                c->type = WL_CELL_BUILTIN;
                c->u.builtin = dict->code->u.builtin;
            }
            ip_tmp = wl_stack_peek(cstack);
            ip_tmp->u.ip->code[ip_tmp->u.ip->pos++] = c;
            break;

        case WL_TOKEN_LIT_NAME:
            c = (struct WlCell*)malloc(sizeof(struct WlCell));
            c->type = WL_CELL_NAME;
            c->u.name = t->u.name;
            ip_tmp = wl_stack_peek(cstack);
            ip_tmp->u.ip->code[ip_tmp->u.ip->pos++] = c;
            break;

        case WL_TOKEN_OPEN_CURLY:
            ip = (struct WlCell*)malloc(sizeof(struct WlCell));
            ip->type = WL_CELL_IP;
            ip->u.ip = (struct WlIp*)malloc(sizeof(struct WlIp));
            ip->u.ip->pos = 0;
            ip->u.ip->code = (struct WlCell**)malloc(sizeof(struct WlCell*) * 1024);
            wl_stack_push(cstack, ip);

            ip_tmp = wl_stack_peek(cstack);
            ip_tmp->u.ip->code[ip_tmp->u.ip->pos++] = c;
            break;

        case WL_TOKEN_CLOSE_CURLY:
            ip_tmp = wl_stack_pop(cstack);
            if (ip_tmp == NULL) {
                printf("too many '}'\n");
                return NULL;
            }
            c = (struct WlCell*)malloc(sizeof(struct WlCell));
            c->type = WL_CELL_PROC;
            c->u.code = ip_tmp->u.ip->code;

            ip_tmp = wl_stack_peek(cstack);
            ip_tmp->u.ip->code[ip_tmp->u.ip->pos++] = c;
            break;

        default:
            ip_tmp = wl_stack_peek(cstack);
            ip_tmp->u.ip->pos++;
        }
    }

    if (cstack->top > 0) {
        printf("too many '{'\n");
        return NULL;
    }
    ip_tmp = wl_stack_peek(cstack);
    ip_tmp->u.ip->code[ip_tmp->u.ip->pos] = NULL;
    return ip_tmp->u.ip->code;
}

void wl_eval(struct WlVm* vm) {
    struct WlStack* cstack = wl_init_stack(1024);
    struct WlCell* ip = (struct WlCell*)malloc(sizeof(struct WlCell));
    wl_stack_push(cstack, ip);

    while (vm->ip->u.ip->code[vm->ip->u.ip->pos] != NULL) {
        struct WlCell* c = vm->ip->u.ip->code[(vm->ip->u.ip->pos)++];
        switch (c->type) {
        case WL_CELL_INT:
        case WL_CELL_NAME:
        case WL_CELL_PROC:
            wl_stack_push(vm->dstack, c);
            break;
        case WL_CELL_CHAR:
            break;
        case WL_CELL_STR:
            break;
        case WL_CELL_BUILTIN:
            printf("builtin!!");
            c->u.builtin(vm);
            break;
        case WL_CELL_IP:
            break;
        }
    }
}

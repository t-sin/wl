#ifndef wl_kernel_h
#define wl_kernel_h

#include <stdint.h>
#include <stdbool.h>

enum WlCellType {
    WL_CELL_TYPE_INT,
    WL_CELL_TYPE_CHAR,
    WL_CELL_TYPE_STR,
    WL_CELL_TYPE_NAME,
    WL_CELL_TYPE_PROC
};

struct WlCell {
    enum WlCellType type;
    union {
        int32_t num;
        char ch;
        char* str;
        char* name;
        uint8_t* code;
    } u;
};

void print_cell(const struct WlCell* c);

struct WlStack {
    int32_t top;
    uint32_t size;
    struct WlCell** stack;
};

void print_stack(const struct WlStack* s);
struct WlStack* wl_init_stack(uint32_t size);
bool wl_stack_full_p(struct WlStack* s);
void wl_stack_resize(struct WlStack* s, uint32_t new_size);
void wl_stack_push(struct WlStack* s, struct WlCell* o);
struct WlCell* wl_stack_pop(struct WlStack* s);
struct WlCell* wl_stack_peek(struct WlStack* s);

struct WlDict {
    char* name;
    struct WlDict* next;
    struct WlCell data;
    uint8_t* code;
};

void print_dict(const struct WlDict* dict);
struct WlDict* dict_find(struct WlDict* dict, char* name);

enum WlTokenType {
    WL_TOKEN_NUMBER,
    WL_TOKEN_CHAR,
    WL_TOKEN_NAME,
    WL_TOKEN_LIT_NAME,
    WL_TOKEN_OPEN_CURLY,
    WL_TOKEN_CLOSE_CURLY,
    WL_TOKEN_NONE,
};

struct WlToken {
    enum WlTokenType type;
    union {
        int32_t num;
        char ch;
        char* str;
        char* name;
    } u;
};

#define NAME_LENGTH 1024

struct WlStream {
    uint32_t idx;
    uint32_t len;
    char* buf;
};

int wl_stream_peek(struct WlStream* s);
int wl_stream_read(struct WlStream* s);
struct WlToken* wl_parse_one(struct WlStream* s);
void print_token(struct WlToken* token);

struct WlVm {
    struct WlDict* dict;
    struct WlStack* dstack;
    struct WlStack* rstack;
};

struct WlVm* wl_init_vm();

void vm_create_dict(struct WlVm* vm);

uint8_t* wl_compile(struct WlToken* tokens);
void wl_eval(struct WlVm* vm);

#endif

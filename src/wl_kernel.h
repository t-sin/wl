#ifndef wl_kernel_h
#define wl_kernel_h

#include <stdint.h>
#include <stdbool.h>

enum WlCellType {
    WL_CELL_TYPE_INT,
    WL_CELL_TYPE_CHAR,
    WL_CELL_TYPE_STR,
    WL_CELL_TYPE_PROC
};

struct WlCell {
    enum WlCellType type;
    union {
        int32_t num;
        char ch;
        char* str;
        uint8_t* code;
    } u;
};

void print_cell(const struct WlCell* c);

struct WlStack {
    uint32_t top;
    uint32_t size;
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
    char* bytecode; 
    struct WlCell data;
    struct WlDict* next;
};

#endif

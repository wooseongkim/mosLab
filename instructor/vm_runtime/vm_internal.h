#ifndef MOSLAB_VM_INTERNAL_H
#define MOSLAB_VM_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

#include "mosvm/vm_machine.h"

#define VM_DEFAULT_PAGE_SIZE 256U
#define VM_DEFAULT_FRAME_COUNT 32U
#define VM_DEFAULT_BLOCK_SIZE 128U
#define VM_DEFAULT_BLOCK_COUNT 64U
#define VM_DEFAULT_CONSOLE_SIZE 4096U

typedef struct vm_machine_impl {
    vm_machine_spec_t spec;
    uint64_t ticks;
    unsigned char *memory;
    unsigned char *blocks;
    char *console;
    size_t console_size;
    size_t console_length;
} vm_machine_impl_t;

vm_machine_impl_t *vm_machine_impl(vm_machine_t *machine);
const vm_machine_impl_t *vm_machine_impl_const(const vm_machine_t *machine);

int vm_checked_span(size_t offset, size_t size, size_t limit);

#endif

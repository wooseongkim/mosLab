#ifndef MOSLAB_VM_INTERNAL_H
#define MOSLAB_VM_INTERNAL_H

#include <stddef.h>

#define VM_DEFAULT_PAGE_SIZE 256U
#define VM_DEFAULT_FRAME_COUNT 32U
#define VM_DEFAULT_BLOCK_SIZE 128U
#define VM_DEFAULT_BLOCK_COUNT 64U
#define VM_DEFAULT_CONSOLE_SIZE 4096U

int vm_checked_span(size_t offset, size_t size, size_t limit);

#endif

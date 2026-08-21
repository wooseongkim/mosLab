#ifndef MOSVM_VM_MEMORY_H
#define MOSVM_VM_MEMORY_H

/**
 * @file vm_memory.h
 * @brief raw physical memory 접근 API.
 */

#include <stddef.h>
#include <stdint.h>

#include "mosvm/vm_error.h"

struct vm_machine;

/**
 * @brief 물리 메모리 바이트를 읽는다.
 */
vm_status_t vm_memory_read(const struct vm_machine *machine,
                           size_t physical_address,
                           void *buffer,
                           size_t size);

/**
 * @brief 물리 메모리 바이트를 쓴다.
 */
vm_status_t vm_memory_write(struct vm_machine *machine,
                            size_t physical_address,
                            const void *buffer,
                            size_t size);

/**
 * @brief 물리 메모리를 0으로 초기화한다.
 */
vm_status_t vm_memory_clear(struct vm_machine *machine);

#endif

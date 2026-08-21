#ifndef MOSVM_VM_BLOCKDEV_H
#define MOSVM_VM_BLOCKDEV_H

/**
 * @file vm_blockdev.h
 * @brief raw block device 접근 API.
 */

#include <stddef.h>

#include "mosvm/vm_error.h"

struct vm_machine;

/**
 * @brief 블록 하나를 읽는다. buffer는 block_size 이상이어야 한다.
 */
vm_status_t vm_block_read(const struct vm_machine *machine,
                          size_t block_index,
                          void *buffer,
                          size_t buffer_size);

/**
 * @brief 블록 하나를 쓴다. buffer는 block_size 이상이어야 한다.
 */
vm_status_t vm_block_write(struct vm_machine *machine,
                           size_t block_index,
                           const void *buffer,
                           size_t buffer_size);

/**
 * @brief 전체 블록 장치를 0으로 초기화한다.
 */
vm_status_t vm_block_reset(struct vm_machine *machine);

#endif

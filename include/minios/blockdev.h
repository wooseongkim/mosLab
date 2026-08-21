#ifndef MINIOS_BLOCKDEV_H
#define MINIOS_BLOCKDEV_H

/**
 * @file blockdev.h
 * @brief LAB7 이전 단계의 block device abstraction API.
 */

#include <stddef.h>

#include "minios/error.h"
#include "mosvm/vm_machine.h"

/** @brief miniOS 블록 장치 핸들. */
typedef struct mos_blockdev {
    vm_machine_t *machine; /**< backing VM. */
    size_t block_size;     /**< 블록 크기. */
    size_t block_count;    /**< 블록 개수. */
} mos_blockdev_t;

/** @brief VM raw block device 위에 miniOS block abstraction을 초기화한다. */
mos_status_t mos_blockdev_init(mos_blockdev_t *device, vm_machine_t *machine);

/** @brief 블록 하나를 읽는다. */
mos_status_t mos_blockdev_read(mos_blockdev_t *device, size_t block_index, void *buffer, size_t buffer_size);

/** @brief 블록 하나를 쓴다. */
mos_status_t mos_blockdev_write(mos_blockdev_t *device, size_t block_index, const void *buffer, size_t buffer_size);

#endif

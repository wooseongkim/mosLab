#ifndef MINIOS_MEMORY_H
#define MINIOS_MEMORY_H

/**
 * @file memory.h
 * @brief LAB5 physical frame allocator API.
 */

#include <stddef.h>

#include "minios/error.h"
#include "minios/kernel.h"
#include "mosvm/vm_machine.h"

/** @brief 물리 프레임 번호. */
typedef size_t mos_frame_t;

/** @brief frame allocator 관찰 통계. */
typedef struct mos_memory_stats {
    size_t total_frames; /**< 전체 프레임 수. */
    size_t free_frames;  /**< 현재 free 프레임 수. */
} mos_memory_stats_t;

/** @brief VM 사양 기반 frame allocator를 초기화한다. */
mos_status_t mos_memory_init(mos_kernel_t *kernel);

/** @brief free frame 하나를 할당한다. */
mos_status_t mos_frame_alloc(mos_kernel_t *kernel, mos_frame_t *frame_out);

/** @brief 할당된 frame을 반납한다. */
mos_status_t mos_frame_free(mos_kernel_t *kernel, mos_frame_t frame);

/** @brief allocator 통계를 조회한다. */
mos_status_t mos_memory_get_stats(const mos_kernel_t *kernel, mos_memory_stats_t *stats_out);

#endif

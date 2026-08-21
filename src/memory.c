#include "minios/memory.h"

/*
 * LAB5 구현 안내
 * - VM spec의 frame_count/page_size를 읽고 free frame 집합을 구성한다.
 * - 중복 free, 범위 밖 frame, allocator 미초기화 상태를 구분해 반환한다.
 * - bitmap, stack, free list 중 하나를 선택하되 public API 밖으로 노출하지 않는다.
 */

mos_status_t mos_memory_init(const vm_machine_t *machine) {
    (void)machine;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_frame_alloc(mos_frame_t *frame_out) {
    (void)frame_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_frame_free(mos_frame_t frame) {
    (void)frame;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_memory_get_stats(mos_memory_stats_t *stats_out) {
    (void)stats_out;
    return MOS_ERR_UNIMPLEMENTED;
}

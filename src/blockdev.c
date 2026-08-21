#include "minios/blockdev.h"

/*
 * LAB7 준비 구현 안내
 * - VM raw block API를 감싼 뒤 miniOS 오류 코드로 변환한다.
 * - buffer_size, block_index, 초기화 여부를 먼저 검사한다.
 * - 상위 파일 시스템이 raw VM 세부사항에 의존하지 않도록 경계를 만든다.
 */

mos_status_t mos_blockdev_init(mos_blockdev_t *device, vm_machine_t *machine) {
    (void)device;
    (void)machine;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_blockdev_read(mos_blockdev_t *device, size_t block_index, void *buffer, size_t buffer_size) {
    (void)device;
    (void)block_index;
    (void)buffer;
    (void)buffer_size;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_blockdev_write(mos_blockdev_t *device, size_t block_index, const void *buffer, size_t buffer_size) {
    (void)device;
    (void)block_index;
    (void)buffer;
    (void)buffer_size;
    return MOS_ERR_UNIMPLEMENTED;
}

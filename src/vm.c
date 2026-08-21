#include "minios/vm.h"

/*
 * LAB6 구현 안내
 * - 가상 페이지 번호와 offset을 분리하고 물리 frame 주소로 변환한다.
 * - page table 구조, writable bit, unmapped page 오류 처리는 학생이 설계한다.
 * - VM 라이브러리의 raw memory API와 혼동하지 말고 주소 변환 정책만 구현한다.
 */

mos_status_t mos_vm_init(void) {
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_vm_map(mos_vaddr_t virtual_page, mos_frame_t frame, int writable) {
    (void)virtual_page;
    (void)frame;
    (void)writable;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_vm_unmap(mos_vaddr_t virtual_page) {
    (void)virtual_page;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_vm_translate(mos_vaddr_t virtual_address, mos_paddr_t *physical_out) {
    (void)virtual_address;
    (void)physical_out;
    return MOS_ERR_UNIMPLEMENTED;
}

#include "minios/system.h"

/*
 * LAB10 구현 안내
 * - LAB1~LAB9 초기화 순서를 통합하고 실패 시 어디까지 성공했는지 report에 기록한다.
 * - integration demo는 public API만 조합해야 하며 private 내부 상태를 직접 만지지 않는다.
 */

mos_status_t mos_system_boot(mos_kernel_t *kernel, mos_system_report_t *report_out) {
    (void)kernel;
    (void)report_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_system_run_demo(mos_kernel_t *kernel, mos_system_report_t *report_out) {
    (void)kernel;
    (void)report_out;
    return MOS_ERR_UNIMPLEMENTED;
}

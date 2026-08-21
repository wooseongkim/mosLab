#include "minios/kernel.h"

/*
 * LAB1 구현 안내
 * - mos_kernel_boot()에서 VM 생성, 커널 상태 전이, 하위 모듈 초기화 순서를 설계한다.
 * - boot를 두 번 호출하거나 shutdown 이후 다시 호출하는 정책을 명확히 정한다.
 * - 모든 포인터 인자와 VM API 반환값을 검사하고 실패 시 상태 코드를 반환한다.
 * - Copilot에게 "커널 lifecycle 상태 전이표를 기준으로 C99 구현"처럼 요청하면 좋다.
 */

mos_status_t mos_kernel_boot(mos_kernel_t *kernel) {
    (void)kernel;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_kernel_shutdown(mos_kernel_t *kernel) {
    (void)kernel;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_kernel_tick(mos_kernel_t *kernel) {
    (void)kernel;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_kernel_ticks(const mos_kernel_t *kernel, uint64_t *ticks_out) {
    (void)kernel;
    (void)ticks_out;
    return MOS_ERR_UNIMPLEMENTED;
}

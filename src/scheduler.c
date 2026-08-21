#include "minios/scheduler.h"

/*
 * LAB3 구현 안내
 * - ready queue와 round-robin 순환 규칙을 직접 구현한다.
 * - scheduler는 VM CPU step만 사용하고 PCB 저장 정책을 대신 제공하지 않는다.
 * - 빈 queue, 중복 enqueue, 종료된 PID 처리 정책을 상태 코드로 표현한다.
 */

mos_status_t mos_scheduler_init(mos_kernel_t *kernel, const mos_scheduler_config_t *config) {
    (void)kernel;
    (void)config;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_scheduler_enqueue(mos_kernel_t *kernel, mos_pid_t pid) {
    (void)kernel;
    (void)pid;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_scheduler_next(mos_kernel_t *kernel, mos_pid_t *pid_out) {
    (void)kernel;
    (void)pid_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_scheduler_run_next(mos_kernel_t *kernel, mos_scheduler_step_result_t *result_out) {
    (void)kernel;
    (void)result_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_scheduler_get_stats(const mos_kernel_t *kernel, mos_scheduler_stats_t *stats_out) {
    (void)kernel;
    (void)stats_out;
    return MOS_ERR_UNIMPLEMENTED;
}

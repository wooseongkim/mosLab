#include "minios/scheduler.h"

/*
 * LAB3 구현 안내
 * - ready queue와 round-robin 순환 규칙을 직접 구현한다.
 * - scheduler는 VM CPU step만 사용하고 PCB 저장 정책을 대신 제공하지 않는다.
 * - 빈 queue, 중복 enqueue, 종료된 PID 처리 정책을 상태 코드로 표현한다.
 */

mos_status_t mos_scheduler_init(const mos_scheduler_config_t *config) {
    (void)config;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_scheduler_enqueue(mos_pid_t pid) {
    (void)pid;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_scheduler_next(mos_pid_t *pid_out) {
    (void)pid_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_scheduler_get_stats(mos_scheduler_stats_t *stats_out) {
    (void)stats_out;
    return MOS_ERR_UNIMPLEMENTED;
}

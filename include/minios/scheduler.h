#ifndef MINIOS_SCHEDULER_H
#define MINIOS_SCHEDULER_H

/**
 * @file scheduler.h
 * @brief LAB3 round-robin scheduler API.
 */

#include "minios/error.h"
#include "minios/process.h"

/** @brief scheduler 초기화 옵션. */
typedef struct mos_scheduler_config {
    unsigned int time_slice_ticks; /**< RR time slice 길이. 0은 유효하지 않다. */
} mos_scheduler_config_t;

/** @brief scheduler 실행 통계. */
typedef struct mos_scheduler_stats {
    unsigned int dispatch_count; /**< dispatch 수행 횟수. */
    mos_pid_t current_pid;       /**< 현재 실행 중인 PID, 없으면 -1. */
} mos_scheduler_stats_t;

/** @brief RR scheduler를 초기화한다. */
mos_status_t mos_scheduler_init(const mos_scheduler_config_t *config);

/** @brief READY 프로세스를 ready queue에 넣는다. */
mos_status_t mos_scheduler_enqueue(mos_pid_t pid);

/** @brief 다음 실행 프로세스를 선택한다. */
mos_status_t mos_scheduler_next(mos_pid_t *pid_out);

/** @brief scheduler 통계를 조회한다. */
mos_status_t mos_scheduler_get_stats(mos_scheduler_stats_t *stats_out);

#endif

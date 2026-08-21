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

/** @brief scheduler가 VM CPU step 후 관찰한 이벤트. */
typedef enum mos_scheduler_event {
    MOS_SCHED_EVENT_NONE = 0,       /**< time slice 내에서 계속 실행 가능. */
    MOS_SCHED_EVENT_YIELD = 1,      /**< 프로세스가 자발적으로 양보함. */
    MOS_SCHED_EVENT_HALTED = 2,     /**< 프로세스가 halt 명령으로 종료됨. */
    MOS_SCHED_EVENT_TRAP = 3,       /**< system call trap이 발생함. */
    MOS_SCHED_EVENT_TIME_SLICE = 4  /**< time slice가 만료됨. */
} mos_scheduler_event_t;

/** @brief scheduler dispatch 결과. */
typedef struct mos_scheduler_step_result {
    mos_pid_t pid;                 /**< dispatch된 PID. */
    mos_scheduler_event_t event;   /**< 관찰된 이벤트. */
    mos_process_state_t new_state; /**< step 후 프로세스 상태. */
} mos_scheduler_step_result_t;

/** @brief RR scheduler를 초기화한다. */
mos_status_t mos_scheduler_init(mos_kernel_t *kernel, const mos_scheduler_config_t *config);

/** @brief READY 프로세스를 ready queue에 넣는다. */
mos_status_t mos_scheduler_enqueue(mos_kernel_t *kernel, mos_pid_t pid);

/** @brief 다음 실행 프로세스를 선택한다. */
mos_status_t mos_scheduler_next(mos_kernel_t *kernel, mos_pid_t *pid_out);

/** @brief 다음 READY 프로세스를 VM CPU에서 최대 time slice만큼 실행한다. */
mos_status_t mos_scheduler_run_next(mos_kernel_t *kernel, mos_scheduler_step_result_t *result_out);

/** @brief scheduler 통계를 조회한다. */
mos_status_t mos_scheduler_get_stats(const mos_kernel_t *kernel, mos_scheduler_stats_t *stats_out);

#endif

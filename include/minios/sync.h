#ifndef MINIOS_SYNC_H
#define MINIOS_SYNC_H

/**
 * @file sync.h
 * @brief LAB4 semaphore와 mutex API.
 */

#include <stddef.h>

#include "minios/error.h"
#include "minios/process.h"

#define MOS_SYNC_WAIT_QUEUE_CAPACITY 16U

/** @brief counting semaphore 공개 표현. */
typedef struct mos_semaphore {
    int value;                                             /**< 현재 카운터 값. */
    mos_pid_t wait_queue[MOS_SYNC_WAIT_QUEUE_CAPACITY];    /**< BLOCKED 대기 PID queue. */
    size_t wait_count;                                     /**< 대기 중인 PID 수. */
} mos_semaphore_t;

/** @brief mutex 공개 표현. */
typedef struct mos_mutex {
    int locked;                                          /**< 잠금 여부. */
    mos_pid_t owner_pid;                                 /**< 현재 소유 PID, 없으면 -1. */
    mos_pid_t wait_queue[MOS_SYNC_WAIT_QUEUE_CAPACITY];  /**< BLOCKED 대기 PID queue. */
    size_t wait_count;                                   /**< 대기 중인 PID 수. */
} mos_mutex_t;

/** @brief semaphore를 초기화한다. */
mos_status_t mos_sem_init(mos_semaphore_t *sem, int initial_value);

/** @brief semaphore wait 연산을 수행한다. */
mos_status_t mos_sem_wait(mos_kernel_t *kernel, mos_semaphore_t *sem, mos_pid_t pid);

/** @brief semaphore signal 연산을 수행한다. */
mos_status_t mos_sem_signal(mos_kernel_t *kernel, mos_semaphore_t *sem);

/** @brief mutex를 초기화한다. */
mos_status_t mos_mutex_init(mos_mutex_t *mutex);

/** @brief mutex lock 연산을 수행한다. */
mos_status_t mos_mutex_lock(mos_kernel_t *kernel, mos_mutex_t *mutex, mos_pid_t pid);

/** @brief mutex unlock 연산을 수행한다. */
mos_status_t mos_mutex_unlock(mos_kernel_t *kernel, mos_mutex_t *mutex, mos_pid_t pid);

#endif

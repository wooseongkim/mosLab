#ifndef MINIOS_SYNC_H
#define MINIOS_SYNC_H

/**
 * @file sync.h
 * @brief LAB4 semaphore와 mutex API.
 */

#include "minios/error.h"
#include "minios/process.h"

/** @brief counting semaphore 공개 표현. */
typedef struct mos_semaphore {
    int value;      /**< 현재 카운터 값. */
    void *waiters;  /**< private wait queue state. */
} mos_semaphore_t;

/** @brief mutex 공개 표현. */
typedef struct mos_mutex {
    int locked;     /**< 잠금 여부. */
    void *waiters;  /**< private wait queue and owner state. */
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

/** @brief mutex 소유 PID를 snapshot으로 조회한다. */
mos_status_t mos_mutex_owner(const mos_mutex_t *mutex, mos_pid_t *pid_out);

#endif

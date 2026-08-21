#ifndef MINIOS_KERNEL_H
#define MINIOS_KERNEL_H

/**
 * @file kernel.h
 * @brief LAB1 kernel lifecycle API.
 */

#include <stdint.h>

#include "minios/error.h"
#include "mosvm/vm_machine.h"

/** @brief 커널 전역 실행 상태. */
typedef enum mos_kernel_state {
    MOS_KERNEL_OFF = 0,     /**< 부팅 전 또는 종료 후. */
    MOS_KERNEL_BOOTED = 1,  /**< VM과 커널 자료구조가 초기화됨. */
    MOS_KERNEL_SHUTDOWN = 2 /**< 정상 종료됨. */
} mos_kernel_state_t;

/** @brief 커널 인스턴스의 공개 관찰 상태. */
typedef struct mos_kernel {
    mos_kernel_state_t state; /**< lifecycle 상태. */
    vm_machine_t machine;     /**< 교수 제공 VM. */
    void *process_table;      /**< LAB2 process table 소유 상태. */
    void *scheduler;          /**< LAB3 scheduler 소유 상태. */
    void *memory;             /**< LAB5 frame allocator 소유 상태. */
    void *virtual_memory;     /**< LAB6 address-space 소유 상태. */
    void *file_system;        /**< LAB7 file system 소유 상태. */
    void *syscalls;           /**< LAB8 syscall table 소유 상태. */
    void *shell;              /**< LAB9 shell 소유 상태. */
} mos_kernel_t;

/** @brief 커널과 VM을 부팅한다. */
mos_status_t mos_kernel_boot(mos_kernel_t *kernel);

/** @brief 커널과 VM 자원을 종료한다. */
mos_status_t mos_kernel_shutdown(mos_kernel_t *kernel);

/** @brief simulated timer를 한 tick 전진한다. */
mos_status_t mos_kernel_tick(mos_kernel_t *kernel);

/** @brief 현재 simulated tick 값을 조회한다. */
mos_status_t mos_kernel_ticks(const mos_kernel_t *kernel, uint64_t *ticks_out);

#endif

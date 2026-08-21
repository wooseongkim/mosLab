#ifndef MINIOS_PROCESS_H
#define MINIOS_PROCESS_H

/**
 * @file process.h
 * @brief LAB2 process API.
 */

#include <stddef.h>

#include "minios/error.h"
#include "mosvm/vm_cpu.h"
#include "mosvm/vm_program.h"

/** @brief 프로세스 식별자 타입. */
typedef int mos_pid_t;

/** @brief 공개 검증에서 관찰하는 프로세스 상태. */
typedef enum mos_process_state {
    MOS_PROC_UNUSED = 0,  /**< PCB 슬롯이 비어 있다. */
    MOS_PROC_READY = 1,   /**< 실행 대기 중이다. */
    MOS_PROC_RUNNING = 2, /**< CPU를 점유하고 있다. */
    MOS_PROC_BLOCKED = 3, /**< 동기화/IO 등으로 대기 중이다. */
    MOS_PROC_EXITED = 4   /**< 종료되었다. */
} mos_process_state_t;

/** @brief 학생이 확장할 수 있는 공개 PCB 시작 형태. */
typedef struct mos_process {
    mos_pid_t pid;                 /**< 프로세스 식별자. */
    mos_process_state_t state;     /**< 현재 상태. */
    vm_cpu_context_t cpu_context;  /**< VM CPU 문맥. */
} mos_process_t;

/** @brief 프로세스 테이블을 초기화한다. */
mos_status_t mos_process_table_init(void);

/** @brief 프로그램 이미지로 새 프로세스를 만든다. */
mos_status_t mos_process_create(const vm_program_t *program, mos_pid_t *pid_out);

/** @brief PID로 프로세스 객체를 조회한다. */
mos_status_t mos_process_get(mos_pid_t pid, mos_process_t **process_out);

/** @brief 프로세스를 종료 상태로 전환한다. */
mos_status_t mos_process_exit(mos_pid_t pid);

/** @brief 현재 할당된 프로세스 수를 조회한다. */
mos_status_t mos_process_count(size_t *count_out);

#endif

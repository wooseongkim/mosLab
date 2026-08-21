#ifndef MOSVM_VM_MACHINE_H
#define MOSVM_VM_MACHINE_H

/**
 * @file vm_machine.h
 * @brief 교수 제공 가상 머신의 lifecycle과 고정 사양 API.
 */

#include <stddef.h>
#include <stdint.h>

#include "mosvm/vm_error.h"

/**
 * @brief VM의 변경 불가능한 하드웨어 사양.
 */
typedef struct vm_machine_spec {
    size_t page_size;      /**< 페이지와 프레임 크기. 기본 256 bytes. */
    size_t frame_count;    /**< 물리 프레임 개수. 기본 32개. */
    size_t block_size;     /**< 블록 장치 블록 크기. 기본 128 bytes. */
    size_t block_count;    /**< 블록 장치 블록 개수. 기본 64개. */
} vm_machine_spec_t;

/**
 * @brief 가상 머신 인스턴스.
 *
 * 구조체 정의는 공개되지만 학생 miniOS는 필드를 직접 수정하지 말고
 * VM API를 통해서만 접근해야 한다.
 */
typedef struct vm_machine {
    vm_machine_spec_t spec;       /**< 고정 하드웨어 사양. */
    uint64_t ticks;               /**< simulated timer tick. */
    unsigned char *memory;        /**< raw physical memory. */
    unsigned char *blocks;        /**< raw block storage. */
    char *console;                /**< deterministic console buffer. */
    size_t console_size;          /**< console capacity including NUL. */
    size_t console_length;        /**< 현재 console 문자열 길이. */
} vm_machine_t;

/**
 * @brief 기본 사양으로 VM을 생성한다.
 */
vm_status_t vm_machine_create(vm_machine_t *machine);

/**
 * @brief VM 자원을 초기 상태로 되돌린다.
 */
vm_status_t vm_machine_reset(vm_machine_t *machine);

/**
 * @brief VM 자원을 해제한다.
 */
vm_status_t vm_machine_destroy(vm_machine_t *machine);

/**
 * @brief VM 고정 사양을 조회한다.
 */
vm_status_t vm_machine_get_spec(const vm_machine_t *machine,
                                vm_machine_spec_t *spec);

/**
 * @brief 현재 simulated tick 값을 조회한다.
 */
vm_status_t vm_machine_get_ticks(const vm_machine_t *machine, uint64_t *ticks);

/**
 * @brief simulated timer를 한 tick 전진한다.
 */
vm_status_t vm_machine_tick(vm_machine_t *machine);

#endif

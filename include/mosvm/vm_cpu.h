#ifndef MOSVM_VM_CPU_H
#define MOSVM_VM_CPU_H

/**
 * @file vm_cpu.h
 * @brief 스케줄러 실습을 위한 단일 가상 CPU API.
 */

#include <stddef.h>
#include <stdint.h>

#include "mosvm/vm_error.h"
#include "mosvm/vm_program.h"

/**
 * @brief vm_cpu_step() 호출 결과로 발생하는 관찰 가능한 이벤트.
 */
typedef enum vm_cpu_event_type {
    VM_CPU_EVENT_NONE = 0,  /**< COMPUTE 실행 후 계속 실행 가능하다. */
    VM_CPU_EVENT_YIELD = 1, /**< YIELD 명령을 만났다. */
    VM_CPU_EVENT_HALT = 2,  /**< HALT 명령을 만났거나 프로그램 끝에 도달했다. */
    VM_CPU_EVENT_TRAP = 3   /**< TRAP 명령을 만났다. */
} vm_cpu_event_type_t;

/**
 * @brief CPU step 결과.
 */
typedef struct vm_cpu_event {
    vm_cpu_event_type_t type; /**< 발생한 이벤트 종류. */
    uint32_t value;           /**< TRAP 번호 등 이벤트 부가 값. */
} vm_cpu_event_t;

/**
 * @brief 학생 OS가 PCB 안에 저장할 수 있는 CPU 문맥.
 *
 * VM은 이 구조체를 제공하지만, 어떤 PCB가 언제 이 문맥을 실행할지는
 * 학생 scheduler가 결정해야 한다.
 */
typedef struct vm_cpu_context {
    const vm_program_t *program; /**< 실행 중인 프로그램 이미지. */
    size_t pc;                   /**< 다음에 실행할 명령 인덱스. */
    uint64_t cycles;             /**< 이 문맥에서 실행한 누적 step 수. */
    int halted;                  /**< HALT 이후 추가 실행을 막기 위한 플래그. */
} vm_cpu_context_t;

/**
 * @brief CPU 문맥을 프로그램 시작 위치로 초기화한다.
 */
vm_status_t vm_cpu_context_init(vm_cpu_context_t *context,
                                const vm_program_t *program);

/**
 * @brief CPU 문맥을 한 명령 실행한다.
 *
 * @param context 실행할 문맥.
 * @param event 결과 이벤트를 받을 객체.
 * @return 성공 또는 오류 상태.
 */
vm_status_t vm_cpu_step(vm_cpu_context_t *context, vm_cpu_event_t *event);

#endif

#ifndef MOSVM_VM_PROGRAM_H
#define MOSVM_VM_PROGRAM_H

/**
 * @file vm_program.h
 * @brief VM CPU가 실행할 단순 명령열 이미지 API.
 */

#include <stddef.h>
#include <stdint.h>

#include "mosvm/vm_error.h"

/**
 * @brief VM이 이해하는 교육용 명령 종류.
 */
typedef enum vm_instruction_opcode {
    VM_INSN_COMPUTE = 1, /**< 한 step 동안 계산을 수행하고 다음 명령으로 이동한다. */
    VM_INSN_YIELD = 2,   /**< CPU가 자발적 양보 이벤트를 발생시킨다. */
    VM_INSN_HALT = 3,    /**< 프로그램 종료 이벤트를 발생시킨다. */
    VM_INSN_TRAP = 4     /**< system call 실습을 위한 trap 이벤트를 발생시킨다. */
} vm_instruction_opcode_t;

/**
 * @brief 하나의 VM 명령.
 */
typedef struct vm_instruction {
    vm_instruction_opcode_t opcode; /**< 실행할 명령 종류. */
    uint32_t operand;               /**< trap 번호 등 확장 실습용 피연산자. */
} vm_instruction_t;

/**
 * @brief 고정 크기 명령열 프로그램 이미지.
 */
typedef struct vm_program {
    const vm_instruction_t *instructions; /**< 호출자가 소유하는 명령 배열. */
    size_t instruction_count;             /**< 배열에 들어 있는 명령 수. */
} vm_program_t;

/**
 * @brief 프로그램 이미지를 초기화한다.
 *
 * @param program 초기화할 프로그램 객체.
 * @param instructions 실행할 명령 배열.
 * @param instruction_count 명령 개수.
 * @return 성공 또는 오류 상태.
 */
vm_status_t vm_program_init(vm_program_t *program,
                            const vm_instruction_t *instructions,
                            size_t instruction_count);

#endif

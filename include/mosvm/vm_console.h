#ifndef MOSVM_VM_CONSOLE_H
#define MOSVM_VM_CONSOLE_H

/**
 * @file vm_console.h
 * @brief 결정적 콘솔 출력 버퍼 API.
 */

#include <stddef.h>

#include "mosvm/vm_error.h"

struct vm_machine;

/**
 * @brief 콘솔에 문자열을 추가한다.
 */
vm_status_t vm_console_write(struct vm_machine *machine, const char *text);

/**
 * @brief 콘솔 버퍼 내용을 조회한다.
 */
vm_status_t vm_console_snapshot(const struct vm_machine *machine,
                                char *buffer,
                                size_t buffer_size);

/**
 * @brief 콘솔 버퍼를 비운다.
 */
vm_status_t vm_console_clear(struct vm_machine *machine);

#endif

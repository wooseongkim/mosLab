#ifndef MINIOS_SYSCALL_H
#define MINIOS_SYSCALL_H

/**
 * @file syscall.h
 * @brief LAB8 FD 기반 system call API.
 */

#include <stddef.h>

#include "minios/error.h"
#include "minios/kernel.h"

/** @brief 파일 디스크립터 타입. */
typedef int mos_fd_t;

/** @brief 지원 system call 번호. */
typedef enum mos_syscall_number {
    MOS_SYS_WRITE = 1, /**< fd에 바이트를 쓴다. */
    MOS_SYS_READ = 2,  /**< fd에서 바이트를 읽는다. */
    MOS_SYS_OPEN = 3,  /**< path를 연다. */
    MOS_SYS_CLOSE = 4  /**< fd를 닫는다. */
} mos_syscall_number_t;

/** @brief syscall 계층을 초기화한다. */
mos_status_t mos_syscall_init(mos_kernel_t *kernel);

/** @brief path를 열고 fd를 반환한다. */
mos_status_t mos_sys_open(mos_kernel_t *kernel, const char *path, mos_fd_t *fd_out);

/** @brief fd에 바이트를 쓴다. */
mos_status_t mos_sys_write(mos_kernel_t *kernel, mos_fd_t fd, const void *buffer, size_t size, size_t *written_out);

/** @brief fd에서 바이트를 읽는다. */
mos_status_t mos_sys_read(mos_kernel_t *kernel, mos_fd_t fd, void *buffer, size_t buffer_size, size_t *read_out);

/** @brief fd를 닫는다. */
mos_status_t mos_sys_close(mos_kernel_t *kernel, mos_fd_t fd);

#endif

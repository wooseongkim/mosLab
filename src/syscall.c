#include "minios/syscall.h"

/*
 * LAB8 구현 안내
 * - fd table을 설계하고 FS/console 같은 대상 객체와 연결한다.
 * - syscall은 policy 계층이므로 VM trap 번호를 어떤 서비스로 해석할지 명확히 한다.
 * - invalid fd, closed fd, buffer 범위 오류를 모두 상태 코드로 표현한다.
 */

mos_status_t mos_syscall_init(void) {
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_sys_open(const char *path, mos_fd_t *fd_out) {
    (void)path;
    (void)fd_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_sys_write(mos_fd_t fd, const void *buffer, size_t size, size_t *written_out) {
    (void)fd;
    (void)buffer;
    (void)size;
    (void)written_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_sys_read(mos_fd_t fd, void *buffer, size_t buffer_size, size_t *read_out) {
    (void)fd;
    (void)buffer;
    (void)buffer_size;
    (void)read_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_sys_close(mos_fd_t fd) {
    (void)fd;
    return MOS_ERR_UNIMPLEMENTED;
}

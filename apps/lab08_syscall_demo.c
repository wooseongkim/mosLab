#include <stdio.h>
#include "minios/kernel.h"
#include "minios/syscall.h"

int main(void) {
    mos_kernel_t kernel;
    mos_fd_t fd = -1;
    size_t written = 0U;

    printf("LAB08 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB08 syscall init status: %d\n", mos_syscall_init(&kernel));
    printf("LAB08 open /log status: %d\n", mos_sys_open(&kernel, "/log", &fd));
    printf("LAB08 write /log status: %d\n", mos_sys_write(&kernel, fd, "abc", 3U, &written));
    printf("LAB08 close /log status: %d\n", mos_sys_close(&kernel, fd));
    printf("LAB08 observed fd=%d written=%lu\n", fd, (unsigned long)written);
    return 0;
}

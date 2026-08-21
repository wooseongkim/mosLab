#include <stdio.h>
#include "minios/fs.h"
#include "minios/kernel.h"
#include "minios/syscall.h"

int main(void) {
    mos_kernel_t kernel;
    mos_blockdev_t device;
    mos_inode_t inode = -1;
    mos_fd_t fd = -1;
    size_t written = 0U;

    printf("LAB08 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB08 block device init status: %d\n", mos_blockdev_init(&device, &kernel.machine));
    printf("LAB08 file system init status: %d\n", mos_fs_init(&kernel, &device));
    printf("LAB08 create /log status: %d\n", mos_fs_create(&kernel, "/log", &inode));
    printf("LAB08 syscall init status: %d\n", mos_syscall_init(&kernel));
    printf("LAB08 open /log status: %d\n", mos_sys_open(&kernel, "/log", &fd));
    printf("LAB08 write /log status: %d\n", mos_sys_write(&kernel, fd, "abc", 3U, &written));
    printf("LAB08 close /log status: %d\n", mos_sys_close(&kernel, fd));
    printf("LAB08 observed inode=%d fd=%d written=%lu\n", inode, fd, (unsigned long)written);
    return 0;
}

#include <stdio.h>
#include "minios/fs.h"
#include "minios/kernel.h"
#include "minios/shell.h"
#include "minios/syscall.h"

int main(void) {
    mos_kernel_t kernel;
    mos_blockdev_t device;
    mos_inode_t inode = -1;
    char output[128];

    printf("LAB09 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB09 block device init status: %d\n", mos_blockdev_init(&device, &kernel.machine));
    printf("LAB09 file system init status: %d\n", mos_fs_init(&kernel, &device));
    printf("LAB09 create /readme status: %d\n", mos_fs_create(&kernel, "/readme", &inode));
    printf("LAB09 syscall init status: %d\n", mos_syscall_init(&kernel));
    printf("LAB09 shell init status: %d\n", mos_shell_init(&kernel));
    printf("LAB09 shell script status: %d\n", mos_shell_run_script(&kernel, "help\nexit\n", output, sizeof(output)));
    printf("LAB09 observed output=%s\n", output);
    return 0;
}

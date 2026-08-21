#include <stdio.h>
#include "minios/blockdev.h"
#include "minios/fs.h"
#include "minios/kernel.h"

int main(void) {
    mos_kernel_t kernel;
    mos_blockdev_t device;
    mos_inode_t inode = -1;
    char buffer[16];
    size_t read_size = 0U;

    printf("LAB07 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB07 block device init status: %d\n", mos_blockdev_init(&device, &kernel.machine));
    printf("LAB07 file system init status: %d\n", mos_fs_init(&kernel, &device));
    printf("LAB07 create /demo status: %d\n", mos_fs_create(&kernel, "/demo", &inode));
    printf("LAB07 write /demo status: %d\n", mos_fs_write(&kernel, "/demo", "os", 2U));
    printf("LAB07 read /demo status: %d\n", mos_fs_read(&kernel, "/demo", buffer, sizeof(buffer), &read_size));
    printf("LAB07 observed inode=%d read_size=%lu\n", inode, (unsigned long)read_size);
    return 0;
}

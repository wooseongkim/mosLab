#include <string.h>
#include "minios/fs.h"
#include "minios/kernel.h"
#include "minios/shell.h"
#include "minios/syscall.h"
#include "test_util.h"

int main(void) {
    mos_kernel_t kernel;
    mos_blockdev_t device;
    mos_inode_t inode = -1;
    char output[128];
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "kernel boots before shell", mos_kernel_boot(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "block device initializes", mos_blockdev_init(&device, &kernel.machine), MOS_OK);
    TEST_EXPECT_STATUS(&report, "file system initializes", mos_fs_init(&kernel, &device), MOS_OK);
    TEST_EXPECT_STATUS(&report, "readme file exists for shell commands", mos_fs_create(&kernel, "/readme", &inode), MOS_OK);
    TEST_EXPECT_STATUS(&report, "syscall layer initializes", mos_syscall_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "shell initializes", mos_shell_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "shell executes help and exit script",
                       mos_shell_run_script(&kernel, "help\nexit\n", output, sizeof(output)),
                       MOS_OK);
    TEST_EXPECT(&report, "shell output mentions help", strstr(output, "help") != 0);

    return test_finish("LAB09", &report);
}

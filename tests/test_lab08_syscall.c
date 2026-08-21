#include <string.h>
#include "minios/fs.h"
#include "minios/kernel.h"
#include "minios/syscall.h"
#include "test_util.h"

int main(void) {
    mos_kernel_t kernel;
    mos_blockdev_t device;
    mos_inode_t inode = -1;
    mos_fd_t fd = -1;
    size_t written = 0U;
    char buffer[8];
    size_t read_size = 0U;
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "system boots before syscall", mos_kernel_boot(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "block device initializes", mos_blockdev_init(&device, &kernel.machine), MOS_OK);
    TEST_EXPECT_STATUS(&report, "file system initializes", mos_fs_init(&kernel, &device), MOS_OK);
    TEST_EXPECT_STATUS(&report, "log file exists before open", mos_fs_create(&kernel, "/log", &inode), MOS_OK);
    TEST_EXPECT_STATUS(&report, "syscall layer initializes", mos_syscall_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "open returns fd", mos_sys_open(&kernel, "/log", &fd), MOS_OK);
    TEST_EXPECT(&report, "fd is non-negative", fd >= 0);
    TEST_EXPECT_STATUS(&report, "write reports written size", mos_sys_write(&kernel, fd, "abc", 3U, &written), MOS_OK);
    TEST_EXPECT(&report, "written size is 3", written == 3U);
    TEST_EXPECT_STATUS(&report, "close fd succeeds", mos_sys_close(&kernel, fd), MOS_OK);
    TEST_EXPECT_STATUS(&report, "closed fd rejects read", mos_sys_read(&kernel, fd, buffer, sizeof(buffer), &read_size), MOS_ERR_INVALID);

    return test_finish("LAB08", &report);
}

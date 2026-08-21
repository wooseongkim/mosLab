#include <string.h>
#include "minios/fs.h"
#include "minios/kernel.h"
#include "test_util.h"

int main(void) {
    mos_kernel_t kernel;
    mos_blockdev_t device;
    mos_inode_t inode = -1;
    char buffer[16];
    size_t read_size = 0U;
    test_report_t report = {0, 0};

    (void)mos_kernel_boot(&kernel);
    TEST_EXPECT_STATUS(&report, "block device initializes", mos_blockdev_init(&device, &kernel.machine), MOS_OK);
    TEST_EXPECT_STATUS(&report, "file system initializes", mos_fs_init(&device), MOS_OK);
    TEST_EXPECT_STATUS(&report, "file can be created", mos_fs_create("/hello", &inode), MOS_OK);
    TEST_EXPECT(&report, "inode is assigned", inode >= 0);
    TEST_EXPECT_STATUS(&report, "file write succeeds", mos_fs_write("/hello", "os", 2U), MOS_OK);
    TEST_EXPECT_STATUS(&report, "file read succeeds", mos_fs_read("/hello", buffer, sizeof(buffer), &read_size), MOS_OK);
    TEST_EXPECT(&report, "file content round trips", read_size == 2U && memcmp(buffer, "os", 2U) == 0);

    return test_finish("LAB07", &report);
}

#include <string.h>
#include "minios/fs.h"
#include "minios/kernel.h"
#include "minios/syscall.h"
#include "test_util.h"

int main(void) {
    mos_fd_t fd = -1;
    size_t written = 0U;
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "syscall layer initializes", mos_syscall_init(), MOS_OK);
    TEST_EXPECT_STATUS(&report, "open returns fd", mos_sys_open("/log", &fd), MOS_OK);
    TEST_EXPECT(&report, "fd is non-negative", fd >= 0);
    TEST_EXPECT_STATUS(&report, "write reports written size", mos_sys_write(fd, "abc", 3U, &written), MOS_OK);
    TEST_EXPECT(&report, "written size is 3", written == 3U);
    TEST_EXPECT_STATUS(&report, "close fd succeeds", mos_sys_close(fd), MOS_OK);

    return test_finish("LAB08", &report);
}

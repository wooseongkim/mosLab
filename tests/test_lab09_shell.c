#include <string.h>
#include "minios/kernel.h"
#include "minios/shell.h"
#include "test_util.h"

int main(void) {
    mos_kernel_t kernel;
    char output[128];
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "kernel boots before shell", mos_kernel_boot(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "shell initializes", mos_shell_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "shell executes help and exit script",
                       mos_shell_run_script(&kernel, "help\nexit\n", output, sizeof(output)),
                       MOS_OK);
    TEST_EXPECT(&report, "shell output mentions help", strstr(output, "help") != 0);

    return test_finish("LAB09", &report);
}

#include "minios/kernel.h"
#include "test_util.h"

int main(void) {
    test_report_t report = {0, 0};
    mos_kernel_t kernel;
    uint64_t ticks = 99U;

    TEST_EXPECT_STATUS(&report, "kernel boots", mos_kernel_boot(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "tick advances after boot", mos_kernel_tick(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "tick value can be read", mos_kernel_ticks(&kernel, &ticks), MOS_OK);
    TEST_EXPECT(&report, "tick value is 1", ticks == 1U);
    TEST_EXPECT_STATUS(&report, "kernel shuts down", mos_kernel_shutdown(&kernel), MOS_OK);

    return test_finish("LAB01", &report);
}

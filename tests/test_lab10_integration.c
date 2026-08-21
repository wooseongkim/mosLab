#include "minios/system.h"
#include "test_util.h"

int main(void) {
    mos_kernel_t kernel;
    mos_system_report_t report_data;
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "system boots all labs", mos_system_boot(&kernel, &report_data), MOS_OK);
    TEST_EXPECT(&report, "all ten labs are ready", report_data.labs_ready == 10U);
    TEST_EXPECT_STATUS(&report, "integration demo runs", mos_system_run_demo(&kernel, &report_data), MOS_OK);
    TEST_EXPECT(&report, "integration demo reports initialized subsystems", report_data.labs_ready >= 10U);

    return test_finish("LAB10", &report);
}

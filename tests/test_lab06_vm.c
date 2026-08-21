#include "minios/vm.h"
#include "test_util.h"

int main(void) {
    mos_kernel_t kernel;
    mos_paddr_t physical1 = 0U;
    mos_paddr_t physical2 = 0U;
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "kernel boots before VM", mos_kernel_boot(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "virtual memory initializes", mos_vm_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 1 maps page to frame 5", mos_vm_map(&kernel, 1, 2U, 5U, 1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 2 maps same page to frame 6", mos_vm_map(&kernel, 2, 2U, 6U, 1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 1 virtual address translates", mos_vm_translate(&kernel, 1, 2U * 256U + 7U, &physical1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 2 virtual address translates", mos_vm_translate(&kernel, 2, 2U * 256U + 7U, &physical2), MOS_OK);
    TEST_EXPECT(&report, "same VA can map to different frames", physical1 == 5U * 256U + 7U && physical2 == 6U * 256U + 7U);
    TEST_EXPECT_STATUS(&report, "pid 1 page unmaps", mos_vm_unmap(&kernel, 1, 2U), MOS_OK);

    return test_finish("LAB06", &report);
}

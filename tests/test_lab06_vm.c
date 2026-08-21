#include "minios/vm.h"
#include "test_util.h"

int main(void) {
    mos_paddr_t physical = 0U;
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "virtual memory initializes", mos_vm_init(), MOS_OK);
    TEST_EXPECT_STATUS(&report, "page maps to frame", mos_vm_map(2U, 5U, 1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "virtual address translates", mos_vm_translate(2U * 256U + 7U, &physical), MOS_OK);
    TEST_EXPECT(&report, "translated physical address keeps offset", physical == 5U * 256U + 7U);
    TEST_EXPECT_STATUS(&report, "page unmaps", mos_vm_unmap(2U), MOS_OK);

    return test_finish("LAB06", &report);
}

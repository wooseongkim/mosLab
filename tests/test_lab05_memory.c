#include "minios/kernel.h"
#include "minios/memory.h"
#include "test_util.h"

int main(void) {
    mos_kernel_t kernel;
    mos_memory_stats_t stats;
    mos_frame_t frame;
    test_report_t report = {0, 0};

    (void)mos_kernel_boot(&kernel);
    TEST_EXPECT_STATUS(&report, "frame allocator initializes from VM spec", mos_memory_init(&kernel.machine), MOS_OK);
    TEST_EXPECT_STATUS(&report, "one frame can be allocated", mos_frame_alloc(&frame), MOS_OK);
    TEST_EXPECT_STATUS(&report, "memory stats can be read", mos_memory_get_stats(&stats), MOS_OK);
    TEST_EXPECT(&report, "free frame count decreases", stats.free_frames + 1U == stats.total_frames);
    TEST_EXPECT_STATUS(&report, "allocated frame can be freed", mos_frame_free(frame), MOS_OK);

    return test_finish("LAB05", &report);
}

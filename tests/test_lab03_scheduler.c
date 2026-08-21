#include "minios/scheduler.h"
#include "test_util.h"

int main(void) {
    mos_scheduler_config_t config = {1U};
    mos_pid_t pid = -1;
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "scheduler initializes", mos_scheduler_init(&config), MOS_OK);
    TEST_EXPECT_STATUS(&report, "enqueue process 1", mos_scheduler_enqueue(1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "enqueue process 2", mos_scheduler_enqueue(2), MOS_OK);
    TEST_EXPECT_STATUS(&report, "enqueue process 3", mos_scheduler_enqueue(3), MOS_OK);
    TEST_EXPECT_STATUS(&report, "RR picks first process", mos_scheduler_next(&pid), MOS_OK);
    TEST_EXPECT(&report, "RR order starts with 1", pid == 1);
    TEST_EXPECT_STATUS(&report, "RR picks second process", mos_scheduler_next(&pid), MOS_OK);
    TEST_EXPECT(&report, "RR order continues with 2", pid == 2);
    TEST_EXPECT_STATUS(&report, "RR picks third process", mos_scheduler_next(&pid), MOS_OK);
    TEST_EXPECT(&report, "RR order continues with 3", pid == 3);

    return test_finish("LAB03", &report);
}

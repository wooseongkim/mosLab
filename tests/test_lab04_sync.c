#include "minios/sync.h"
#include "test_util.h"

int main(void) {
    mos_semaphore_t sem;
    mos_mutex_t mutex;
    test_report_t report = {0, 0};

    TEST_EXPECT_STATUS(&report, "semaphore initializes with value 1", mos_sem_init(&sem, 1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "semaphore wait consumes value", mos_sem_wait(&sem), MOS_OK);
    TEST_EXPECT_STATUS(&report, "semaphore signal restores value", mos_sem_signal(&sem), MOS_OK);
    TEST_EXPECT_STATUS(&report, "mutex initializes unlocked", mos_mutex_init(&mutex), MOS_OK);
    TEST_EXPECT_STATUS(&report, "mutex lock succeeds", mos_mutex_lock(&mutex), MOS_OK);
    TEST_EXPECT_STATUS(&report, "mutex unlock succeeds", mos_mutex_unlock(&mutex), MOS_OK);

    return test_finish("LAB04", &report);
}

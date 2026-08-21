#include "minios/sync.h"
#include "test_util.h"

int main(void) {
    const vm_instruction_t instructions[] = {{VM_INSN_HALT, 0U}};
    vm_program_t program;
    mos_kernel_t kernel;
    mos_pid_t owner = -1;
    mos_pid_t waiter = -1;
    mos_process_info_t waiter_info;
    mos_semaphore_t sem;
    mos_mutex_t mutex;
    test_report_t report = {0, 0};

    (void)vm_program_init(&program, instructions, 1U);
    TEST_EXPECT_STATUS(&report, "kernel boots before sync", mos_kernel_boot(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "process table initializes", mos_process_table_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "owner process can be created", mos_process_create(&kernel, &program, &owner), MOS_OK);
    TEST_EXPECT_STATUS(&report, "waiter process can be created", mos_process_create(&kernel, &program, &waiter), MOS_OK);
    TEST_EXPECT_STATUS(&report, "semaphore initializes with value 1", mos_sem_init(&sem, 1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "semaphore wait consumes value", mos_sem_wait(&kernel, &sem, owner), MOS_OK);
    TEST_EXPECT_STATUS(&report, "second waiter blocks on empty semaphore", mos_sem_wait(&kernel, &sem, waiter), MOS_ERR_BLOCKED);
    TEST_EXPECT_STATUS(&report, "blocked process snapshot can be read", mos_process_info(&kernel, waiter, &waiter_info), MOS_OK);
    TEST_EXPECT(&report, "semaphore wait marks process BLOCKED", waiter_info.state == MOS_PROC_BLOCKED);
    TEST_EXPECT_STATUS(&report, "semaphore signal wakes waiter", mos_sem_signal(&kernel, &sem), MOS_OK);
    TEST_EXPECT_STATUS(&report, "mutex initializes unlocked", mos_mutex_init(&mutex), MOS_OK);
    TEST_EXPECT_STATUS(&report, "mutex lock records owner", mos_mutex_lock(&kernel, &mutex, owner), MOS_OK);
    TEST_EXPECT(&report, "mutex owner is visible", mutex.owner_pid == owner);
    TEST_EXPECT_STATUS(&report, "second locker blocks", mos_mutex_lock(&kernel, &mutex, waiter), MOS_ERR_BLOCKED);
    TEST_EXPECT_STATUS(&report, "mutex unlock by owner succeeds", mos_mutex_unlock(&kernel, &mutex, owner), MOS_OK);

    return test_finish("LAB04", &report);
}

#include "minios/scheduler.h"
#include "test_util.h"

int main(void) {
    const vm_instruction_t yield_program_code[] = {{VM_INSN_YIELD, 0U}, {VM_INSN_HALT, 0U}};
    mos_scheduler_config_t config = {1U};
    vm_program_t program;
    mos_kernel_t kernel;
    mos_pid_t pid1 = -1;
    mos_pid_t pid2 = -1;
    mos_pid_t pid = -1;
    mos_scheduler_step_result_t step;
    test_report_t report = {0, 0};

    (void)vm_program_init(&program, yield_program_code, 2U);
    TEST_EXPECT_STATUS(&report, "kernel boots before scheduler", mos_kernel_boot(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "process table initializes", mos_process_table_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "first process can be created", mos_process_create(&kernel, &program, &pid1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "second process can be created", mos_process_create(&kernel, &program, &pid2), MOS_OK);
    TEST_EXPECT_STATUS(&report, "scheduler initializes", mos_scheduler_init(&kernel, &config), MOS_OK);
    TEST_EXPECT_STATUS(&report, "enqueue first process", mos_scheduler_enqueue(&kernel, pid1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "enqueue second process", mos_scheduler_enqueue(&kernel, pid2), MOS_OK);
    TEST_EXPECT_STATUS(&report, "RR picks first process", mos_scheduler_next(&kernel, &pid), MOS_OK);
    TEST_EXPECT(&report, "RR order starts with first PID", pid == pid1);
    TEST_EXPECT_STATUS(&report, "RR picks second process", mos_scheduler_next(&kernel, &pid), MOS_OK);
    TEST_EXPECT(&report, "RR order continues with second PID", pid == pid2);
    TEST_EXPECT_STATUS(&report, "scheduler can step VM CPU", mos_scheduler_run_next(&kernel, &step), MOS_OK);
    TEST_EXPECT(&report, "yield event is reported", step.event == MOS_SCHED_EVENT_YIELD);

    return test_finish("LAB03", &report);
}

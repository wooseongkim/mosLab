#include "minios/process.h"
#include "test_util.h"

int main(void) {
    const vm_instruction_t instructions[] = {{VM_INSN_HALT, 0U}};
    vm_program_t program;
    mos_pid_t pid = -1;
    mos_process_t *process = 0;
    size_t count = 0U;
    test_report_t report = {0, 0};

    (void)vm_program_init(&program, instructions, 1U);
    TEST_EXPECT_STATUS(&report, "process table initializes", mos_process_table_init(), MOS_OK);
    TEST_EXPECT_STATUS(&report, "process can be created", mos_process_create(&program, &pid), MOS_OK);
    TEST_EXPECT(&report, "assigned PID is positive", pid > 0);
    TEST_EXPECT_STATUS(&report, "process can be looked up", mos_process_get(pid, &process), MOS_OK);
    TEST_EXPECT(&report, "created process starts READY", process != 0 && process->state == MOS_PROC_READY);
    TEST_EXPECT_STATUS(&report, "process count is observable", mos_process_count(&count), MOS_OK);
    TEST_EXPECT(&report, "process count is one", count == 1U);

    return test_finish("LAB02", &report);
}

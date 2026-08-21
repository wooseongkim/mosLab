#include "minios/vm.h"
#include "test_util.h"

int main(void) {
    const vm_instruction_t instructions[] = {{VM_INSN_HALT, 0U}};
    vm_program_t program;
    mos_kernel_t kernel;
    mos_pid_t pid1 = -1;
    mos_pid_t pid2 = -1;
    mos_frame_t frame1 = 0U;
    mos_frame_t frame2 = 0U;
    mos_paddr_t physical1 = 0U;
    mos_paddr_t physical2 = 0U;
    test_report_t report = {0, 0};

    (void)vm_program_init(&program, instructions, 1U);
    TEST_EXPECT_STATUS(&report, "kernel boots before VM", mos_kernel_boot(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "process table initializes", mos_process_table_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 1 process is created", mos_process_create(&kernel, &program, &pid1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 2 process is created", mos_process_create(&kernel, &program, &pid2), MOS_OK);
    TEST_EXPECT_STATUS(&report, "frame allocator initializes", mos_memory_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "first frame is allocated", mos_frame_alloc(&kernel, &frame1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "second frame is allocated", mos_frame_alloc(&kernel, &frame2), MOS_OK);
    TEST_EXPECT_STATUS(&report, "virtual memory initializes", mos_vm_init(&kernel), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 1 maps page to allocated frame", mos_vm_map(&kernel, pid1, 2U, frame1, 1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 2 maps same page to another allocated frame", mos_vm_map(&kernel, pid2, 2U, frame2, 1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 1 virtual address translates", mos_vm_translate(&kernel, pid1, 2U * MOS_VM_PAGE_SIZE + 7U, &physical1), MOS_OK);
    TEST_EXPECT_STATUS(&report, "pid 2 virtual address translates", mos_vm_translate(&kernel, pid2, 2U * MOS_VM_PAGE_SIZE + 7U, &physical2), MOS_OK);
    TEST_EXPECT(&report, "same VA can map to different frames",
                physical1 == frame1 * MOS_VM_PAGE_SIZE + 7U &&
                physical2 == frame2 * MOS_VM_PAGE_SIZE + 7U &&
                frame1 != frame2);
    TEST_EXPECT_STATUS(&report, "pid 1 page unmaps", mos_vm_unmap(&kernel, pid1, 2U), MOS_OK);

    return test_finish("LAB06", &report);
}

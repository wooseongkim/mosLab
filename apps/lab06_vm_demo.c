#include <stdio.h>
#include "minios/kernel.h"
#include "minios/memory.h"
#include "minios/process.h"
#include "minios/vm.h"

int main(void) {
    const vm_instruction_t instructions[] = {{VM_INSN_HALT, 0U}};
    vm_program_t program;
    mos_kernel_t kernel;
    mos_pid_t pid = -1;
    mos_frame_t frame = 0U;
    mos_paddr_t physical = 0U;

    (void)vm_program_init(&program, instructions, 1U);
    printf("LAB06 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB06 process table init status: %d\n", mos_process_table_init(&kernel));
    printf("LAB06 process create status: %d\n", mos_process_create(&kernel, &program, &pid));
    printf("LAB06 memory init status: %d\n", mos_memory_init(&kernel));
    printf("LAB06 frame alloc status: %d\n", mos_frame_alloc(&kernel, &frame));
    printf("LAB06 virtual memory init status: %d\n", mos_vm_init(&kernel));
    printf("LAB06 map status: %d\n", mos_vm_map(&kernel, pid, 2U, frame, 1));
    printf("LAB06 translate status: %d\n",
           mos_vm_translate(&kernel, pid, 2U * MOS_VM_PAGE_SIZE + 7U, &physical));
    printf("LAB06 observed pid=%d frame=%lu physical=%lu\n",
           pid,
           (unsigned long)frame,
           (unsigned long)physical);
    return 0;
}

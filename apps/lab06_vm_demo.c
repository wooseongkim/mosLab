#include <stdio.h>
#include "minios/kernel.h"
#include "minios/memory.h"
#include "minios/vm.h"

int main(void) {
    mos_kernel_t kernel;
    mos_frame_t frame = 0U;
    mos_paddr_t physical = 0U;

    printf("LAB06 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB06 memory init status: %d\n", mos_memory_init(&kernel));
    printf("LAB06 frame alloc status: %d\n", mos_frame_alloc(&kernel, &frame));
    printf("LAB06 virtual memory init status: %d\n", mos_vm_init(&kernel));
    printf("LAB06 map status: %d\n", mos_vm_map(&kernel, 1, 2U, frame, 1));
    printf("LAB06 translate status: %d\n",
           mos_vm_translate(&kernel, 1, 2U * MOS_VM_PAGE_SIZE + 7U, &physical));
    printf("LAB06 observed physical=%lu\n", (unsigned long)physical);
    return 0;
}

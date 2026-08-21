#include <stdio.h>
#include "minios/kernel.h"
#include "minios/vm.h"

int main(void) {
    mos_kernel_t kernel;
    printf("LAB06 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB06 virtual memory init status: %d\n", mos_vm_init(&kernel));
    return 0;
}

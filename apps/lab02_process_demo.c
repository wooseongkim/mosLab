#include <stdio.h>
#include "minios/kernel.h"
#include "minios/process.h"

int main(void) {
    mos_kernel_t kernel;
    printf("LAB02 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB02 process init status: %d\n", mos_process_table_init(&kernel));
    return 0;
}

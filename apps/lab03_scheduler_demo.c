#include <stdio.h>
#include "minios/kernel.h"
#include "minios/scheduler.h"

int main(void) {
    mos_kernel_t kernel;
    mos_scheduler_config_t config = {2U};
    printf("LAB03 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB03 scheduler init status: %d\n", mos_scheduler_init(&kernel, &config));
    return 0;
}

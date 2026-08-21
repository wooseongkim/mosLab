#include <stdio.h>
#include "minios/system.h"

int main(void) {
    mos_kernel_t kernel;
    mos_system_report_t report;
    printf("LAB10 system boot status: %d\n", mos_system_boot(&kernel, &report));
    return 0;
}

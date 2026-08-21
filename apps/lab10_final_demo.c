#include <stdio.h>
#include "minios/system.h"

int main(void) {
    mos_kernel_t kernel;
    mos_system_report_t report;
    printf("LAB10 system boot status: %d\n", mos_system_boot(&kernel, &report));
    printf("LAB10 labs ready after boot: %u\n", report.labs_ready);
    printf("LAB10 integration demo status: %d\n", mos_system_run_demo(&kernel, &report));
    printf("LAB10 labs ready after demo: %u\n", report.labs_ready);
    return 0;
}

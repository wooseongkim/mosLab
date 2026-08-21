#include <stdio.h>
#include "minios/kernel.h"

int main(void) {
    mos_kernel_t kernel;
    mos_status_t status = mos_kernel_boot(&kernel);
    printf("LAB01 boot status: %d\n", status);
    if (status == MOS_OK) {
        (void)mos_kernel_shutdown(&kernel);
    }
    return status == MOS_OK ? 0 : 1;
}

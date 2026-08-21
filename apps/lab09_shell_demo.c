#include <stdio.h>
#include "minios/kernel.h"
#include "minios/shell.h"

int main(void) {
    mos_kernel_t kernel;
    char output[128];
    printf("LAB09 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB09 shell init status: %d\n", mos_shell_init(&kernel));
    printf("LAB09 shell script status: %d\n", mos_shell_run_script(&kernel, "help\nexit\n", output, sizeof(output)));
    return 0;
}

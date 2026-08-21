#include <stdio.h>
#include "minios/shell.h"

int main(void) {
    printf("LAB09 shell init status: %d\n", mos_shell_init());
    return 0;
}

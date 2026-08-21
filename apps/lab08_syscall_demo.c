#include <stdio.h>
#include "minios/syscall.h"

int main(void) {
    printf("LAB08 syscall init status: %d\n", mos_syscall_init());
    return 0;
}

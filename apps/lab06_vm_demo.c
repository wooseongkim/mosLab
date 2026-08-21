#include <stdio.h>
#include "minios/vm.h"

int main(void) {
    printf("LAB06 virtual memory init status: %d\n", mos_vm_init());
    return 0;
}

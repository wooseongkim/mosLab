#include <stdio.h>
#include "minios/process.h"

int main(void) {
    printf("LAB02 process init status: %d\n", mos_process_table_init());
    return 0;
}

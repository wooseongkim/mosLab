#include <stdio.h>
#include "minios/scheduler.h"

int main(void) {
    mos_scheduler_config_t config = {2U};
    printf("LAB03 scheduler init status: %d\n", mos_scheduler_init(&config));
    return 0;
}

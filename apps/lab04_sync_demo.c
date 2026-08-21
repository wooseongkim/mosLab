#include <stdio.h>
#include "minios/sync.h"

int main(void) {
    mos_semaphore_t sem;
    printf("LAB04 semaphore init status: %d\n", mos_sem_init(&sem, 1));
    return 0;
}

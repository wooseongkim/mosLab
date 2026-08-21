#include <stdio.h>
#include "minios/kernel.h"
#include "minios/sync.h"

int main(void) {
    mos_kernel_t kernel;
    mos_semaphore_t sem;
    printf("LAB04 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB04 semaphore init status: %d\n", mos_sem_init(&sem, 1));
    printf("LAB04 semaphore wait status: %d\n", mos_sem_wait(&kernel, &sem, 1));
    return 0;
}

#include <stdio.h>
#include "minios/kernel.h"
#include "minios/memory.h"

int main(void) {
    mos_kernel_t kernel;
    mos_memory_stats_t stats;
    mos_frame_t frame = 0U;

    printf("LAB05 kernel boot status: %d\n", mos_kernel_boot(&kernel));
    printf("LAB05 memory init status: %d\n", mos_memory_init(&kernel));
    printf("LAB05 frame alloc status: %d\n", mos_frame_alloc(&kernel, &frame));
    printf("LAB05 memory stats status: %d\n", mos_memory_get_stats(&kernel, &stats));
    printf("LAB05 observed frame=%lu total=%lu free=%lu\n",
           (unsigned long)frame,
           (unsigned long)stats.total_frames,
           (unsigned long)stats.free_frames);
    return 0;
}

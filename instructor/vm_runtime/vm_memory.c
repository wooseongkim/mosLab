#include "mosvm/vm_memory.h"

#include <string.h>

#include "instructor/vm_runtime/vm_internal.h"
#include "mosvm/vm_machine.h"

vm_status_t vm_memory_read(const vm_machine_t *machine,
                           size_t physical_address,
                           void *buffer,
                           size_t size) {
    size_t memory_size;

    if (machine == NULL || buffer == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->memory == NULL) {
        return VM_ERR_STATE;
    }

    memory_size = machine->spec.page_size * machine->spec.frame_count;
    if (!vm_checked_span(physical_address, size, memory_size)) {
        return VM_ERR_RANGE;
    }

    memcpy(buffer, machine->memory + physical_address, size);
    return VM_OK;
}

vm_status_t vm_memory_write(vm_machine_t *machine,
                            size_t physical_address,
                            const void *buffer,
                            size_t size) {
    size_t memory_size;

    if (machine == NULL || buffer == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->memory == NULL) {
        return VM_ERR_STATE;
    }

    memory_size = machine->spec.page_size * machine->spec.frame_count;
    if (!vm_checked_span(physical_address, size, memory_size)) {
        return VM_ERR_RANGE;
    }

    memcpy(machine->memory + physical_address, buffer, size);
    return VM_OK;
}

vm_status_t vm_memory_clear(vm_machine_t *machine) {
    size_t memory_size;

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->memory == NULL) {
        return VM_ERR_STATE;
    }

    memory_size = machine->spec.page_size * machine->spec.frame_count;
    memset(machine->memory, 0, memory_size);
    return VM_OK;
}

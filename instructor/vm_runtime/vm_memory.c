#include "mosvm/vm_memory.h"

#include <string.h>

#include "instructor/vm_runtime/vm_internal.h"
#include "mosvm/vm_machine.h"

vm_status_t vm_memory_read(const vm_machine_t *machine,
                           size_t physical_address,
                           void *buffer,
                           size_t size) {
    size_t memory_size;
    const vm_machine_impl_t *impl = vm_machine_impl_const(machine);

    if (machine == NULL || buffer == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->memory == NULL) {
        return VM_ERR_STATE;
    }

    memory_size = impl->spec.page_size * impl->spec.frame_count;
    if (!vm_checked_span(physical_address, size, memory_size)) {
        return VM_ERR_RANGE;
    }

    memcpy(buffer, impl->memory + physical_address, size);
    return VM_OK;
}

vm_status_t vm_memory_write(vm_machine_t *machine,
                            size_t physical_address,
                            const void *buffer,
                            size_t size) {
    size_t memory_size;
    vm_machine_impl_t *impl = vm_machine_impl(machine);

    if (machine == NULL || buffer == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->memory == NULL) {
        return VM_ERR_STATE;
    }

    memory_size = impl->spec.page_size * impl->spec.frame_count;
    if (!vm_checked_span(physical_address, size, memory_size)) {
        return VM_ERR_RANGE;
    }

    memcpy(impl->memory + physical_address, buffer, size);
    return VM_OK;
}

vm_status_t vm_memory_clear(vm_machine_t *machine) {
    size_t memory_size;
    vm_machine_impl_t *impl = vm_machine_impl(machine);

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->memory == NULL) {
        return VM_ERR_STATE;
    }

    memory_size = impl->spec.page_size * impl->spec.frame_count;
    memset(impl->memory, 0, memory_size);
    return VM_OK;
}

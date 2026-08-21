#include "mosvm/vm_blockdev.h"

#include <string.h>

#include "instructor/vm_runtime/vm_internal.h"
#include "mosvm/vm_machine.h"

static vm_status_t vm_block_validate(const vm_machine_t *machine,
                                     size_t block_index,
                                     size_t buffer_size) {
    const vm_machine_impl_t *impl = vm_machine_impl_const(machine);

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->blocks == NULL) {
        return VM_ERR_STATE;
    }
    if (block_index >= impl->spec.block_count) {
        return VM_ERR_RANGE;
    }
    if (buffer_size < impl->spec.block_size) {
        return VM_ERR_BAD_ARG;
    }
    return VM_OK;
}

vm_status_t vm_block_read(const vm_machine_t *machine,
                          size_t block_index,
                          void *buffer,
                          size_t buffer_size) {
    vm_status_t status;
    const vm_machine_impl_t *impl = vm_machine_impl_const(machine);
    size_t offset;

    if (buffer == NULL) {
        return VM_ERR_NULL;
    }
    status = vm_block_validate(machine, block_index, buffer_size);
    if (status != VM_OK) {
        return status;
    }

    offset = block_index * impl->spec.block_size;
    memcpy(buffer, impl->blocks + offset, impl->spec.block_size);
    return VM_OK;
}

vm_status_t vm_block_write(vm_machine_t *machine,
                           size_t block_index,
                           const void *buffer,
                           size_t buffer_size) {
    vm_status_t status;
    vm_machine_impl_t *impl = vm_machine_impl(machine);
    size_t offset;

    if (buffer == NULL) {
        return VM_ERR_NULL;
    }
    status = vm_block_validate(machine, block_index, buffer_size);
    if (status != VM_OK) {
        return status;
    }

    offset = block_index * impl->spec.block_size;
    memcpy(impl->blocks + offset, buffer, impl->spec.block_size);
    return VM_OK;
}

vm_status_t vm_block_reset(vm_machine_t *machine) {
    size_t block_bytes;
    vm_machine_impl_t *impl = vm_machine_impl(machine);

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->blocks == NULL) {
        return VM_ERR_STATE;
    }

    block_bytes = impl->spec.block_size * impl->spec.block_count;
    memset(impl->blocks, 0, block_bytes);
    return VM_OK;
}

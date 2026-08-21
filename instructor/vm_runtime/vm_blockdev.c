#include "mosvm/vm_blockdev.h"

#include <string.h>

#include "instructor/vm_runtime/vm_internal.h"
#include "mosvm/vm_machine.h"

static vm_status_t vm_block_validate(const vm_machine_t *machine,
                                     size_t block_index,
                                     size_t buffer_size) {
    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->blocks == NULL) {
        return VM_ERR_STATE;
    }
    if (block_index >= machine->spec.block_count) {
        return VM_ERR_RANGE;
    }
    if (buffer_size < machine->spec.block_size) {
        return VM_ERR_BAD_ARG;
    }
    return VM_OK;
}

vm_status_t vm_block_read(const vm_machine_t *machine,
                          size_t block_index,
                          void *buffer,
                          size_t buffer_size) {
    vm_status_t status;
    size_t offset;

    if (buffer == NULL) {
        return VM_ERR_NULL;
    }
    status = vm_block_validate(machine, block_index, buffer_size);
    if (status != VM_OK) {
        return status;
    }

    offset = block_index * machine->spec.block_size;
    memcpy(buffer, machine->blocks + offset, machine->spec.block_size);
    return VM_OK;
}

vm_status_t vm_block_write(vm_machine_t *machine,
                           size_t block_index,
                           const void *buffer,
                           size_t buffer_size) {
    vm_status_t status;
    size_t offset;

    if (buffer == NULL) {
        return VM_ERR_NULL;
    }
    status = vm_block_validate(machine, block_index, buffer_size);
    if (status != VM_OK) {
        return status;
    }

    offset = block_index * machine->spec.block_size;
    memcpy(machine->blocks + offset, buffer, machine->spec.block_size);
    return VM_OK;
}

vm_status_t vm_block_reset(vm_machine_t *machine) {
    size_t block_bytes;

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->blocks == NULL) {
        return VM_ERR_STATE;
    }

    block_bytes = machine->spec.block_size * machine->spec.block_count;
    memset(machine->blocks, 0, block_bytes);
    return VM_OK;
}

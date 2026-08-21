#include "mosvm/vm_machine.h"

#include <stdlib.h>
#include <string.h>

#include "instructor/vm_runtime/vm_internal.h"

int vm_checked_span(size_t offset, size_t size, size_t limit) {
    return offset <= limit && size <= (limit - offset);
}

static void vm_machine_zero_public_fields(vm_machine_t *machine) {
    machine->spec.page_size = 0U;
    machine->spec.frame_count = 0U;
    machine->spec.block_size = 0U;
    machine->spec.block_count = 0U;
    machine->ticks = 0U;
    machine->memory = NULL;
    machine->blocks = NULL;
    machine->console = NULL;
    machine->console_size = 0U;
    machine->console_length = 0U;
}

vm_status_t vm_machine_create(vm_machine_t *machine) {
    size_t memory_size;
    size_t block_bytes;

    if (machine == NULL) {
        return VM_ERR_NULL;
    }

    vm_machine_zero_public_fields(machine);
    machine->spec.page_size = VM_DEFAULT_PAGE_SIZE;
    machine->spec.frame_count = VM_DEFAULT_FRAME_COUNT;
    machine->spec.block_size = VM_DEFAULT_BLOCK_SIZE;
    machine->spec.block_count = VM_DEFAULT_BLOCK_COUNT;
    machine->console_size = VM_DEFAULT_CONSOLE_SIZE;

    memory_size = machine->spec.page_size * machine->spec.frame_count;
    block_bytes = machine->spec.block_size * machine->spec.block_count;

    machine->memory = (unsigned char *)calloc(memory_size, sizeof(unsigned char));
    machine->blocks = (unsigned char *)calloc(block_bytes, sizeof(unsigned char));
    machine->console = (char *)calloc(machine->console_size, sizeof(char));

    if (machine->memory == NULL || machine->blocks == NULL || machine->console == NULL) {
        (void)vm_machine_destroy(machine);
        return VM_ERR_STATE;
    }

    return VM_OK;
}

vm_status_t vm_machine_reset(vm_machine_t *machine) {
    size_t memory_size;
    size_t block_bytes;

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->memory == NULL || machine->blocks == NULL || machine->console == NULL) {
        return VM_ERR_STATE;
    }

    memory_size = machine->spec.page_size * machine->spec.frame_count;
    block_bytes = machine->spec.block_size * machine->spec.block_count;
    memset(machine->memory, 0, memory_size);
    memset(machine->blocks, 0, block_bytes);
    memset(machine->console, 0, machine->console_size);
    machine->console_length = 0U;
    machine->ticks = 0U;

    return VM_OK;
}

vm_status_t vm_machine_destroy(vm_machine_t *machine) {
    if (machine == NULL) {
        return VM_ERR_NULL;
    }

    free(machine->memory);
    free(machine->blocks);
    free(machine->console);
    vm_machine_zero_public_fields(machine);

    return VM_OK;
}

vm_status_t vm_machine_get_spec(const vm_machine_t *machine, vm_machine_spec_t *spec) {
    if (machine == NULL || spec == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->memory == NULL || machine->blocks == NULL || machine->console == NULL) {
        return VM_ERR_STATE;
    }

    *spec = machine->spec;
    return VM_OK;
}

vm_status_t vm_machine_get_ticks(const vm_machine_t *machine, uint64_t *ticks) {
    if (machine == NULL || ticks == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->memory == NULL) {
        return VM_ERR_STATE;
    }

    *ticks = machine->ticks;
    return VM_OK;
}

vm_status_t vm_machine_tick(vm_machine_t *machine) {
    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->memory == NULL) {
        return VM_ERR_STATE;
    }

    machine->ticks++;
    return VM_OK;
}

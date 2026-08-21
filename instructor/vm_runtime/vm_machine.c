#include "mosvm/vm_machine.h"

#include <stdlib.h>
#include <string.h>

#include "instructor/vm_runtime/vm_internal.h"

int vm_checked_span(size_t offset, size_t size, size_t limit) {
    return offset <= limit && size <= (limit - offset);
}

vm_machine_impl_t *vm_machine_impl(vm_machine_t *machine) {
    return machine == NULL ? NULL : (vm_machine_impl_t *)machine->impl;
}

const vm_machine_impl_t *vm_machine_impl_const(const vm_machine_t *machine) {
    return machine == NULL ? NULL : (const vm_machine_impl_t *)machine->impl;
}

static void vm_machine_free_impl(vm_machine_impl_t *impl) {
    if (impl == NULL) {
        return;
    }
    free(impl->memory);
    free(impl->blocks);
    free(impl->console);
    free(impl);
}

vm_status_t vm_machine_create(vm_machine_t *machine) {
    vm_machine_impl_t *impl;
    size_t memory_size;
    size_t block_bytes;

    if (machine == NULL) {
        return VM_ERR_NULL;
    }

    machine->impl = NULL;
    impl = (vm_machine_impl_t *)calloc(1U, sizeof(*impl));
    if (impl == NULL) {
        return VM_ERR_STATE;
    }

    impl->spec.page_size = VM_DEFAULT_PAGE_SIZE;
    impl->spec.frame_count = VM_DEFAULT_FRAME_COUNT;
    impl->spec.block_size = VM_DEFAULT_BLOCK_SIZE;
    impl->spec.block_count = VM_DEFAULT_BLOCK_COUNT;
    impl->console_size = VM_DEFAULT_CONSOLE_SIZE;

    memory_size = impl->spec.page_size * impl->spec.frame_count;
    block_bytes = impl->spec.block_size * impl->spec.block_count;

    impl->memory = (unsigned char *)calloc(memory_size, sizeof(unsigned char));
    impl->blocks = (unsigned char *)calloc(block_bytes, sizeof(unsigned char));
    impl->console = (char *)calloc(impl->console_size, sizeof(char));

    if (impl->memory == NULL || impl->blocks == NULL || impl->console == NULL) {
        vm_machine_free_impl(impl);
        return VM_ERR_STATE;
    }

    machine->impl = impl;
    return VM_OK;
}

vm_status_t vm_machine_reset(vm_machine_t *machine) {
    vm_machine_impl_t *impl = vm_machine_impl(machine);
    size_t memory_size;
    size_t block_bytes;

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->memory == NULL || impl->blocks == NULL || impl->console == NULL) {
        return VM_ERR_STATE;
    }

    memory_size = impl->spec.page_size * impl->spec.frame_count;
    block_bytes = impl->spec.block_size * impl->spec.block_count;
    memset(impl->memory, 0, memory_size);
    memset(impl->blocks, 0, block_bytes);
    memset(impl->console, 0, impl->console_size);
    impl->console_length = 0U;
    impl->ticks = 0U;

    return VM_OK;
}

vm_status_t vm_machine_destroy(vm_machine_t *machine) {
    if (machine == NULL) {
        return VM_ERR_NULL;
    }

    vm_machine_free_impl(vm_machine_impl(machine));
    machine->impl = NULL;
    return VM_OK;
}

vm_status_t vm_machine_get_spec(const vm_machine_t *machine, vm_machine_spec_t *spec) {
    const vm_machine_impl_t *impl = vm_machine_impl_const(machine);

    if (machine == NULL || spec == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->memory == NULL || impl->blocks == NULL || impl->console == NULL) {
        return VM_ERR_STATE;
    }

    *spec = impl->spec;
    return VM_OK;
}

vm_status_t vm_machine_get_ticks(const vm_machine_t *machine, uint64_t *ticks) {
    const vm_machine_impl_t *impl = vm_machine_impl_const(machine);

    if (machine == NULL || ticks == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->memory == NULL) {
        return VM_ERR_STATE;
    }

    *ticks = impl->ticks;
    return VM_OK;
}

vm_status_t vm_machine_tick(vm_machine_t *machine) {
    vm_machine_impl_t *impl = vm_machine_impl(machine);

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->memory == NULL) {
        return VM_ERR_STATE;
    }

    impl->ticks++;
    return VM_OK;
}

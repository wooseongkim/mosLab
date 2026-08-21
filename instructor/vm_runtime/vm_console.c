#include "mosvm/vm_console.h"

#include <string.h>

#include "instructor/vm_runtime/vm_internal.h"
#include "mosvm/vm_machine.h"

vm_status_t vm_console_write(vm_machine_t *machine, const char *text) {
    vm_machine_impl_t *impl = vm_machine_impl(machine);
    size_t length;

    if (machine == NULL || text == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->console == NULL || impl->console_size == 0U) {
        return VM_ERR_STATE;
    }

    length = strlen(text);
    if (length >= impl->console_size - impl->console_length) {
        return VM_ERR_FULL;
    }

    memcpy(impl->console + impl->console_length, text, length + 1U);
    impl->console_length += length;
    return VM_OK;
}

vm_status_t vm_console_snapshot(const vm_machine_t *machine,
                                char *buffer,
                                size_t buffer_size) {
    const vm_machine_impl_t *impl = vm_machine_impl_const(machine);

    if (machine == NULL || buffer == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->console == NULL) {
        return VM_ERR_STATE;
    }
    if (buffer_size == 0U || buffer_size <= impl->console_length) {
        return VM_ERR_BAD_ARG;
    }

    memcpy(buffer, impl->console, impl->console_length + 1U);
    return VM_OK;
}

vm_status_t vm_console_clear(vm_machine_t *machine) {
    vm_machine_impl_t *impl = vm_machine_impl(machine);

    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (impl == NULL || impl->console == NULL) {
        return VM_ERR_STATE;
    }

    memset(impl->console, 0, impl->console_size);
    impl->console_length = 0U;
    return VM_OK;
}

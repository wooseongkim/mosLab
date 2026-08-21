#include "mosvm/vm_console.h"

#include <string.h>

#include "mosvm/vm_machine.h"

vm_status_t vm_console_write(vm_machine_t *machine, const char *text) {
    size_t length;

    if (machine == NULL || text == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->console == NULL || machine->console_size == 0U) {
        return VM_ERR_STATE;
    }

    length = strlen(text);
    if (length >= machine->console_size - machine->console_length) {
        return VM_ERR_FULL;
    }

    memcpy(machine->console + machine->console_length, text, length + 1U);
    machine->console_length += length;
    return VM_OK;
}

vm_status_t vm_console_snapshot(const vm_machine_t *machine,
                                char *buffer,
                                size_t buffer_size) {
    if (machine == NULL || buffer == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->console == NULL) {
        return VM_ERR_STATE;
    }
    if (buffer_size == 0U || buffer_size <= machine->console_length) {
        return VM_ERR_BAD_ARG;
    }

    memcpy(buffer, machine->console, machine->console_length + 1U);
    return VM_OK;
}

vm_status_t vm_console_clear(vm_machine_t *machine) {
    if (machine == NULL) {
        return VM_ERR_NULL;
    }
    if (machine->console == NULL) {
        return VM_ERR_STATE;
    }

    memset(machine->console, 0, machine->console_size);
    machine->console_length = 0U;
    return VM_OK;
}

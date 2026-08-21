#include "mosvm/vm_program.h"

vm_status_t vm_program_init(vm_program_t *program,
                            const vm_instruction_t *instructions,
                            size_t instruction_count) {
    if (program == NULL || instructions == NULL) {
        return VM_ERR_NULL;
    }
    if (instruction_count == 0U) {
        return VM_ERR_BAD_ARG;
    }

    program->instructions = instructions;
    program->instruction_count = instruction_count;
    return VM_OK;
}

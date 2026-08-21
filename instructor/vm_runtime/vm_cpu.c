#include "mosvm/vm_cpu.h"

vm_status_t vm_cpu_context_init(vm_cpu_context_t *context,
                                const vm_program_t *program) {
    if (context == NULL || program == NULL) {
        return VM_ERR_NULL;
    }
    if (program->instructions == NULL || program->instruction_count == 0U) {
        return VM_ERR_BAD_ARG;
    }

    context->program = program;
    context->pc = 0U;
    context->cycles = 0U;
    context->halted = 0;
    return VM_OK;
}

vm_status_t vm_cpu_step(vm_cpu_context_t *context, vm_cpu_event_t *event) {
    vm_instruction_t instruction;

    if (context == NULL || event == NULL) {
        return VM_ERR_NULL;
    }
    if (context->program == NULL || context->program->instructions == NULL) {
        return VM_ERR_STATE;
    }
    if (context->halted) {
        return VM_ERR_STATE;
    }

    event->type = VM_CPU_EVENT_NONE;
    event->value = 0U;

    if (context->pc >= context->program->instruction_count) {
        context->halted = 1;
        event->type = VM_CPU_EVENT_HALT;
        return VM_OK;
    }

    instruction = context->program->instructions[context->pc];
    context->pc++;
    context->cycles++;

    switch (instruction.opcode) {
        case VM_INSN_COMPUTE:
            event->type = VM_CPU_EVENT_NONE;
            break;
        case VM_INSN_YIELD:
            event->type = VM_CPU_EVENT_YIELD;
            break;
        case VM_INSN_HALT:
            context->halted = 1;
            event->type = VM_CPU_EVENT_HALT;
            break;
        case VM_INSN_TRAP:
            event->type = VM_CPU_EVENT_TRAP;
            event->value = instruction.operand;
            break;
        default:
            return VM_ERR_BAD_ARG;
    }

    return VM_OK;
}

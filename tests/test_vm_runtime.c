#include <stdint.h>
#include <string.h>

#include "mosvm/vm_blockdev.h"
#include "mosvm/vm_console.h"
#include "mosvm/vm_cpu.h"
#include "mosvm/vm_machine.h"
#include "mosvm/vm_memory.h"
#include "mosvm/vm_program.h"
#include "test_util.h"

static void test_machine_defaults(test_report_t *report) {
    vm_machine_t machine;
    vm_machine_spec_t spec;
    uint64_t ticks;

    TEST_EXPECT_STATUS(report, "create default VM", vm_machine_create(&machine), VM_OK);
    TEST_EXPECT_STATUS(report, "read VM spec", vm_machine_get_spec(&machine, &spec), VM_OK);
    TEST_EXPECT(report, "default page size is 256", spec.page_size == 256U);
    TEST_EXPECT(report, "default physical memory has 32 frames", spec.frame_count == 32U);
    TEST_EXPECT(report, "default block size is 128", spec.block_size == 128U);
    TEST_EXPECT(report, "default block device has 64 blocks", spec.block_count == 64U);
    TEST_EXPECT_STATUS(report, "initial tick read succeeds", vm_machine_get_ticks(&machine, &ticks), VM_OK);
    TEST_EXPECT(report, "initial tick is zero", ticks == 0U);
    TEST_EXPECT_STATUS(report, "one tick advances timer", vm_machine_tick(&machine), VM_OK);
    TEST_EXPECT_STATUS(report, "advanced tick read succeeds", vm_machine_get_ticks(&machine, &ticks), VM_OK);
    TEST_EXPECT(report, "tick value is one", ticks == 1U);
    TEST_EXPECT_STATUS(report, "destroy default VM", vm_machine_destroy(&machine), VM_OK);
}

static void test_memory_and_block_bounds(test_report_t *report) {
    vm_machine_t machine;
    unsigned char write_bytes[4] = {1U, 2U, 3U, 4U};
    unsigned char read_bytes[4] = {0U, 0U, 0U, 0U};
    unsigned char block[128];
    vm_machine_spec_t spec;
    size_t memory_size;

    memset(block, 0x5A, sizeof(block));
    TEST_EXPECT_STATUS(report, "create VM for storage tests", vm_machine_create(&machine), VM_OK);
    TEST_EXPECT_STATUS(report, "read spec for bounds tests", vm_machine_get_spec(&machine, &spec), VM_OK);
    memory_size = spec.page_size * spec.frame_count;

    TEST_EXPECT_STATUS(report, "write physical memory bytes",
                       vm_memory_write(&machine, memory_size - sizeof(write_bytes),
                                       write_bytes, sizeof(write_bytes)),
                       VM_OK);
    TEST_EXPECT_STATUS(report, "read physical memory bytes",
                       vm_memory_read(&machine, memory_size - sizeof(read_bytes),
                                      read_bytes, sizeof(read_bytes)),
                       VM_OK);
    TEST_EXPECT(report, "memory bytes round trip", memcmp(write_bytes, read_bytes, sizeof(write_bytes)) == 0);
    TEST_EXPECT_STATUS(report, "reject memory write past end",
                       vm_memory_write(&machine, memory_size - 1U, write_bytes, sizeof(write_bytes)),
                       VM_ERR_RANGE);

    TEST_EXPECT_STATUS(report, "write last block",
                       vm_block_write(&machine, spec.block_count - 1U, block, sizeof(block)),
                       VM_OK);
    memset(block, 0, sizeof(block));
    TEST_EXPECT_STATUS(report, "read last block",
                       vm_block_read(&machine, spec.block_count - 1U, block, sizeof(block)),
                       VM_OK);
    TEST_EXPECT(report, "block data round trip", block[0] == 0x5A && block[127] == 0x5A);
    TEST_EXPECT_STATUS(report, "reject block index past end",
                       vm_block_read(&machine, spec.block_count, block, sizeof(block)),
                       VM_ERR_RANGE);

    TEST_EXPECT_STATUS(report, "destroy VM after storage tests", vm_machine_destroy(&machine), VM_OK);
}

static void test_cpu_and_console(test_report_t *report) {
    const vm_instruction_t instructions[] = {
        {VM_INSN_COMPUTE, 0U},
        {VM_INSN_TRAP, 7U},
        {VM_INSN_YIELD, 0U},
        {VM_INSN_HALT, 0U}
    };
    vm_program_t program;
    vm_cpu_context_t context;
    vm_cpu_event_t event;
    vm_machine_t machine;
    char console[32];

    TEST_EXPECT_STATUS(report, "initialize program image",
                       vm_program_init(&program, instructions,
                                       sizeof(instructions) / sizeof(instructions[0])),
                       VM_OK);
    TEST_EXPECT_STATUS(report, "initialize CPU context", vm_cpu_context_init(&context, &program), VM_OK);
    TEST_EXPECT_STATUS(report, "compute step succeeds", vm_cpu_step(&context, &event), VM_OK);
    TEST_EXPECT(report, "compute step produces no event", event.type == VM_CPU_EVENT_NONE);
    TEST_EXPECT_STATUS(report, "trap step succeeds", vm_cpu_step(&context, &event), VM_OK);
    TEST_EXPECT(report, "trap event carries operand", event.type == VM_CPU_EVENT_TRAP && event.value == 7U);
    TEST_EXPECT_STATUS(report, "yield step succeeds", vm_cpu_step(&context, &event), VM_OK);
    TEST_EXPECT(report, "yield event is observable", event.type == VM_CPU_EVENT_YIELD);
    TEST_EXPECT_STATUS(report, "halt step succeeds", vm_cpu_step(&context, &event), VM_OK);
    TEST_EXPECT(report, "halt event is observable", event.type == VM_CPU_EVENT_HALT);
    TEST_EXPECT_STATUS(report, "halted context rejects extra step", vm_cpu_step(&context, &event), VM_ERR_STATE);

    TEST_EXPECT_STATUS(report, "create VM for console", vm_machine_create(&machine), VM_OK);
    TEST_EXPECT_STATUS(report, "write deterministic console output",
                       vm_console_write(&machine, "boot\n"), VM_OK);
    TEST_EXPECT_STATUS(report, "snapshot console output",
                       vm_console_snapshot(&machine, console, sizeof(console)), VM_OK);
    TEST_EXPECT(report, "console snapshot matches written bytes", strcmp(console, "boot\n") == 0);
    TEST_EXPECT_STATUS(report, "destroy VM after console", vm_machine_destroy(&machine), VM_OK);
}

int main(void) {
    test_report_t report = {0, 0};

    test_machine_defaults(&report);
    test_memory_and_block_bounds(&report);
    test_cpu_and_console(&report);

    return test_finish("VM RUNTIME", &report);
}

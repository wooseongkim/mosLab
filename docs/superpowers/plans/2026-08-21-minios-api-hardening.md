# miniOS API Hardening Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Prepare the C miniOS lab for distribution by tightening API ownership, cumulative tests, build hygiene, and student-facing docs.

**Architecture:** Keep the repository as a student lab distribution with stubs in `src/`, but make public contracts force kernel-owned state instead of hidden globals. Public tests should exercise state transitions and cumulative LAB setup without requiring a reference solution in the tree.

**Tech Stack:** C99, GNU Make, professor-provided `mosvm` runtime, shell scripts.

**Spec:** `docs/superpowers/specs/2026-08-20-agentic-minios-lab-design.md`

## Global Constraints

- Student implementation lives in `src/`.
- Public API lives in `include/minios/`.
- Public tests must use public headers only.
- Keep `lib/libmosvm.a` available for student environments.
- Do not reintroduce the removed Python miniOS implementation.

---

### Task 1: Kernel-Owned Public API

**Files:**
- Modify: `include/minios/kernel.h`
- Modify: `include/minios/process.h`
- Modify: `include/minios/scheduler.h`
- Modify: `include/minios/sync.h`
- Modify: `include/minios/vm.h`
- Modify: `include/minios/fs.h`
- Modify: `include/minios/syscall.h`
- Modify: `include/minios/shell.h`
- Modify: `include/minios/system.h`
- Modify: `src/*.c`

**Interfaces:**
- Consumes: `mos_kernel_t *kernel`
- Produces: public APIs that require `mos_kernel_t *` for mutable miniOS state

- [ ] Write failing compile checks by updating public tests to call kernel-owned APIs.
- [ ] Run `make test-lab02` and confirm compilation fails because old API signatures are still present.
- [ ] Update public headers to add `mos_kernel_t *kernel` parameters where module state is owned by miniOS.
- [ ] Update source stubs to match signatures and keep returning `MOS_ERR_UNIMPLEMENTED`.
- [ ] Run `make test-lab02` and confirm the test compiles and fails at runtime on the stub implementation.

### Task 2: Blocking Sync Contract

**Files:**
- Modify: `include/minios/sync.h`
- Modify: `tests/test_lab04_sync.c`
- Modify: `src/sync.c`

**Interfaces:**
- Consumes: `mos_kernel_t *kernel`, `mos_pid_t pid`
- Produces: semaphore and mutex contracts that can express BLOCKED/READY transitions

- [ ] Update LAB4 tests to create two processes and verify a wait/lock call can block the second process.
- [ ] Run `make test-lab04` and confirm the old API cannot compile.
- [ ] Add wait queue fields to `mos_semaphore_t` and `mos_mutex_t`.
- [ ] Add owner PID to `mos_mutex_t`.
- [ ] Update `mos_sem_wait`, `mos_sem_signal`, `mos_mutex_lock`, and `mos_mutex_unlock` signatures to include kernel and PID context.
- [ ] Run `make test-lab04` and confirm the test compiles and fails at runtime on the stub implementation.

### Task 3: Per-Process Virtual Memory Contract

**Files:**
- Modify: `include/minios/vm.h`
- Modify: `tests/test_lab06_vm.c`
- Modify: `src/vm.c`

**Interfaces:**
- Consumes: `mos_pid_t pid`
- Produces: mappings scoped by process address space

- [ ] Update LAB6 tests to map the same virtual page for two different PIDs to two different frames.
- [ ] Run `make test-lab06` and confirm the old API cannot compile.
- [ ] Update VM signatures to include PID for map, unmap, and translate.
- [ ] Add `mos_address_space_t` and a page-table capacity contract to `vm.h`.
- [ ] Run `make test-lab06` and confirm the test compiles and fails at runtime on the stub implementation.

### Task 4: Scheduler CPU-Step Contract

**Files:**
- Modify: `include/minios/scheduler.h`
- Modify: `tests/test_lab03_scheduler.c`
- Modify: `src/scheduler.c`

**Interfaces:**
- Consumes: VM CPU events from `vm_cpu_step`
- Produces: explicit run result for yield, halt, trap, and time-slice expiration

- [ ] Update LAB3 tests to create processes through LAB1/LAB2 setup and call scheduler through `mos_kernel_t`.
- [ ] Add `mos_scheduler_step_result_t` and `mos_scheduler_run_next`.
- [ ] Run `make test-lab03` and confirm stubs compile but fail runtime checks.

### Task 5: Build and Repository Hygiene

**Files:**
- Create: `.gitignore`
- Modify: `Makefile`
- Remove from working tree: `build/`, `tests/__pycache__/`

**Interfaces:**
- Consumes: make targets
- Produces: `make help`, `make syntax-check`, stronger `make verify-instructor`

- [ ] Add `.gitignore` entries for build outputs, object files, executables, and Python caches.
- [ ] Add `help` target that lists supported commands.
- [ ] Add `syntax-check` target that runs `$(CC) $(CFLAGS) -fsyntax-only` against headers, apps, tests, and `src/`.
- [ ] Make `verify-instructor` depend on `syntax-check` and VM runtime tests.
- [ ] Remove generated build/cache artifacts from the workspace.
- [ ] Run `make help` and `make verify-instructor`.

### Task 6: Docs and Demo Workloads

**Files:**
- Modify: `docs/instructor/lab00_environment.md`
- Modify: `apps/lab07_fs_demo.c`
- Modify: `apps/lab08_syscall_demo.c`

**Interfaces:**
- Consumes: student commands and public APIs
- Produces: clearer setup flow and demos that exercise actual APIs

- [ ] Add Copilot login, Student activation, VS Code Chat/Agent usage, change review, and failed-test feedback loop instructions to LAB0.
- [ ] Update LAB7 demo to initialize kernel and filesystem, create/write/read a file, and print observed state.
- [ ] Update LAB8 demo to initialize kernel/filesystem/syscall, open/write/close/read back through public API where available.
- [ ] Run `make syntax-check`.

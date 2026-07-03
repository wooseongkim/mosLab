from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any

from minios.kernel.syscall import Syscall, SyscallError
from minios.process.pcb import PCB, ProcessState


@dataclass
class Console:
    output: bytes = b""

    def write(self, data: bytes) -> int:
        if not isinstance(data, bytes):
            raise TypeError("console accepts bytes")
        self.output += data
        return len(data)


@dataclass
class KernelTrace:
    transitions: list[tuple[int, ProcessState, ProcessState]] = field(default_factory=list)
    max_simultaneous_running: int = 0


class Kernel:
    def __init__(self, machine) -> None:
        self.machine = machine
        self.booted = False
        self.console = Console()
        self.processes: dict[int, PCB] = {}
        self.trace = KernelTrace()
        self._next_pid = 1

    def boot(self) -> None:
        self.booted = True

    def invoke(self, syscall_number: int, *args: Any):
        if not self.booted:
            raise SyscallError("kernel is not booted")
        try:
            syscall = Syscall(syscall_number)
        except ValueError as exc:
            raise SyscallError(f"unknown syscall: {syscall_number}") from exc
        if syscall is Syscall.CONSOLE_WRITE:
            if len(args) != 1:
                raise SyscallError("CONSOLE_WRITE requires one argument")
            return self.console.write(args[0])
        raise SyscallError(f"unimplemented syscall: {syscall.name}")

    def spawn(self, program: list[tuple[Any, ...]]) -> int:
        if not self.booted:
            raise RuntimeError("kernel is not booted")
        pid = self._next_pid
        self._next_pid += 1
        pcb = PCB(pid=pid, program=list(program))
        self.processes[pid] = pcb
        self._transition(pcb, ProcessState.READY)
        return pid

    def run(self, *, max_ticks: int) -> None:
        if max_ticks <= 0:
            raise ValueError("max_ticks must be positive")
        ticks = 0
        while ticks < max_ticks:
            ready = next((pcb for pcb in self.processes.values() if pcb.state is ProcessState.READY), None)
            if ready is None:
                break
            self._transition(ready, ProcessState.RUNNING)
            self.trace.max_simultaneous_running = max(
                self.trace.max_simultaneous_running,
                sum(pcb.state is ProcessState.RUNNING for pcb in self.processes.values()),
            )
            self._execute_tick(ready)
            ticks += 1
            if ready.state is ProcessState.RUNNING:
                self._transition(ready, ProcessState.READY)

    def wait(self, pid: int) -> int:
        pcb = self.processes.get(pid)
        if pcb is None:
            raise KeyError(pid)
        if pcb.state is not ProcessState.TERMINATED or pcb.exit_code is None:
            raise RuntimeError("process has not terminated")
        return pcb.exit_code

    def _execute_tick(self, pcb: PCB) -> None:
        if pcb.pc >= len(pcb.program):
            pcb.exit_code = 0
            self._transition(pcb, ProcessState.TERMINATED)
            return
        instruction = pcb.program[pcb.pc]
        opcode = instruction[0]
        if opcode == "COMPUTE":
            remaining = pcb.context.get("compute_remaining", int(instruction[1]))
            if remaining <= 0:
                raise ValueError("COMPUTE burst must be positive")
            remaining -= 1
            if remaining == 0:
                pcb.context.pop("compute_remaining", None)
                pcb.pc += 1
            else:
                pcb.context["compute_remaining"] = remaining
        elif opcode == "EXIT":
            pcb.exit_code = int(instruction[1])
            pcb.pc += 1
            self._transition(pcb, ProcessState.TERMINATED)
        else:
            raise ValueError(f"unknown process opcode: {opcode}")

    def _transition(self, pcb: PCB, state: ProcessState) -> None:
        previous = pcb.state
        pcb.state = state
        self.trace.transitions.append((pcb.pid, previous, state))

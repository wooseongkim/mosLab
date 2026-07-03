from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
from typing import Any


@dataclass(frozen=True)
class InterruptEvent:
    kind: str
    tick: int


@dataclass
class MachineTrace:
    halted: bool = False
    ticks: int = 0
    interrupts: list[InterruptEvent] = field(default_factory=list)


class Memory:
    def __init__(self, size: int) -> None:
        if size <= 0:
            raise ValueError("RAM size must be positive")
        self._data = bytearray(size)

    @property
    def size(self) -> int:
        return len(self._data)

    def read8(self, address: int) -> int:
        self._check(address)
        return self._data[address]

    def write8(self, address: int, value: int) -> None:
        self._check(address)
        if not 0 <= value <= 255:
            raise ValueError("byte value must be between 0 and 255")
        self._data[address] = value

    def _check(self, address: int) -> None:
        if not 0 <= address < len(self._data):
            raise IndexError(f"memory address out of range: {address}")


class FileDisk:
    def __init__(self, path: Path, block_size: int, block_count: int) -> None:
        if block_size <= 0 or block_count <= 0:
            raise ValueError("block geometry must be positive")
        self.path = Path(path)
        self.block_size = block_size
        self.block_count = block_count
        self.path.parent.mkdir(parents=True, exist_ok=True)
        required = block_size * block_count
        if not self.path.exists():
            self.path.write_bytes(bytes(required))
        elif self.path.stat().st_size < required:
            with self.path.open("ab") as stream:
                stream.write(bytes(required - self.path.stat().st_size))

    def read_block(self, block: int) -> bytes:
        self._check(block)
        with self.path.open("rb") as stream:
            stream.seek(block * self.block_size)
            data = stream.read(self.block_size)
        if len(data) != self.block_size:
            raise OSError("short block read")
        return data

    def write_block(self, block: int, data: bytes) -> None:
        self._check(block)
        if len(data) != self.block_size:
            raise ValueError(f"block write requires {self.block_size} bytes")
        with self.path.open("r+b") as stream:
            stream.seek(block * self.block_size)
            stream.write(data)

    def _check(self, block: int) -> None:
        if not 0 <= block < self.block_count:
            raise IndexError(f"block out of range: {block}")


class CPU:
    def __init__(self, memory: Memory) -> None:
        self.memory = memory
        self.registers: dict[str, int] = {f"R{i}": 0 for i in range(4)}
        self.pc = 0
        self.halted = False

    def reset(self) -> None:
        self.registers = {f"R{i}": 0 for i in range(4)}
        self.pc = 0
        self.halted = False

    def step(self, program: list[tuple[Any, ...]]) -> None:
        if self.halted:
            return
        if not 0 <= self.pc < len(program):
            raise RuntimeError("program counter left the program")
        instruction = program[self.pc]
        opcode = instruction[0]
        if opcode == "MOVI":
            _, register, value = instruction
            self._register(register)
            self.registers[register] = int(value)
            self.pc += 1
        elif opcode == "STORE8":
            _, register, address = instruction
            self._register(register)
            self.memory.write8(int(address), self.registers[register])
            self.pc += 1
        elif opcode == "LOAD8":
            _, register, address = instruction
            self._register(register)
            self.registers[register] = self.memory.read8(int(address))
            self.pc += 1
        elif opcode == "ADD":
            _, register, value = instruction
            self._register(register)
            self.registers[register] += int(value)
            self.pc += 1
        elif opcode == "HALT":
            self.halted = True
        else:
            raise ValueError(f"unknown opcode: {opcode}")

    def _register(self, name: str) -> None:
        if name not in self.registers:
            raise ValueError(f"unknown register: {name}")


class Machine:
    def __init__(
        self,
        *,
        ram_size: int,
        block_size: int,
        block_count: int,
        disk_path: Path,
        timer_period: int,
    ) -> None:
        if timer_period <= 0:
            raise ValueError("timer period must be positive")
        self.memory = Memory(ram_size)
        self.disk = FileDisk(disk_path, block_size, block_count)
        self.cpu = CPU(self.memory)
        self.timer_period = timer_period
        self.program: list[tuple[Any, ...]] = []

    def load_program(self, instructions: list[tuple[Any, ...]]) -> None:
        self.program = list(instructions)
        self.cpu.reset()

    def run(self, *, max_ticks: int) -> MachineTrace:
        if max_ticks <= 0:
            raise ValueError("max_ticks must be positive")
        trace = MachineTrace()
        while not self.cpu.halted and trace.ticks < max_ticks:
            self.cpu.step(self.program)
            trace.ticks += 1
            if trace.ticks % self.timer_period == 0:
                trace.interrupts.append(InterruptEvent("TIMER", trace.ticks))
        trace.halted = self.cpu.halted
        return trace


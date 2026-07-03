from __future__ import annotations

import shlex
from dataclasses import dataclass
from pathlib import Path

from minios.fs.filesystem import FileSystem
from minios.hardware.machine import Machine
from minios.io.block import FileBlockDevice
from minios.kernel.kernel import Kernel


@dataclass
class CommandResult:
    exit_code: int
    stdout: str = ""
    pid: int | None = None


class Shell:
    def __init__(self, system: "MiniOS") -> None:
        self.system = system

    def execute(self, command: str) -> CommandResult:
        parts = shlex.split(command)
        if not parts:
            return CommandResult(0)
        if parts[0] == "echo" and ">" in parts:
            redirect = parts.index(">")
            if redirect < 2 or redirect + 2 != len(parts):
                return CommandResult(2, "invalid echo syntax\n")
            text = " ".join(parts[1:redirect]).encode("utf-8")
            path = parts[redirect + 1]
            if path in self.system.fs.list_paths():
                self.system.fs.unlink(path)
            fd = self.system.fs.create(path)
            self.system.fs.write(fd, text)
            self.system.fs.close(fd)
            return CommandResult(0)
        if parts[0] == "cat" and len(parts) == 2:
            try:
                fd = self.system.fs.open(parts[1])
                data = self.system.fs.read(fd, self.system.device.total_size)
                self.system.fs.close(fd)
            except FileNotFoundError:
                return CommandResult(1, f"cat: {parts[1]}: not found\n")
            return CommandResult(0, data.decode("utf-8") + "\n")
        if parts[0] == "run" and len(parts) == 3 and parts[1] == "count":
            count = int(parts[2])
            pid = self.system.kernel.spawn([("COMPUTE", count), ("EXIT", 0)])
            return CommandResult(0, pid=pid)
        if parts[0] == "ls" and len(parts) == 1:
            output = "\n".join(self.system.fs.list_paths())
            return CommandResult(0, output + ("\n" if output else ""))
        if parts[0] == "rm" and len(parts) == 2:
            try:
                self.system.fs.unlink(parts[1])
            except FileNotFoundError:
                return CommandResult(1, f"rm: {parts[1]}: not found\n")
            return CommandResult(0)
        if parts[0] == "ps" and len(parts) == 1:
            lines = [f"{pid} {pcb.state.name}" for pid, pcb in sorted(self.system.kernel.processes.items())]
            return CommandResult(0, "\n".join(lines) + ("\n" if lines else ""))
        return CommandResult(127, f"unknown command: {parts[0]}\n")


class MiniOS:
    BLOCK_SIZE = 128
    BLOCK_COUNT = 256

    def __init__(self, *, disk_path: Path, ram_size: int) -> None:
        self.disk_path = Path(disk_path)
        self.ram_size = ram_size
        self.machine: Machine | None = None
        self.device: FileBlockDevice | None = None
        self.kernel: Kernel | None = None
        self.fs: FileSystem | None = None
        self.shell: Shell | None = None

    def boot(self) -> None:
        if self.kernel is not None:
            raise RuntimeError("MiniOS is already booted")
        self.machine = Machine(
            ram_size=self.ram_size,
            block_size=self.BLOCK_SIZE,
            block_count=self.BLOCK_COUNT,
            disk_path=self.disk_path,
            timer_period=2,
        )
        self.device = FileBlockDevice(
            self.disk_path,
            block_size=self.BLOCK_SIZE,
            block_count=self.BLOCK_COUNT,
        )
        if not FileSystem.is_formatted(self.device):
            FileSystem.format(self.device, inode_count=64)
        self.fs = FileSystem.mount(self.device)
        self.kernel = Kernel(self.machine)
        self.kernel.boot()
        self.shell = Shell(self)

    def run(self, *, max_ticks: int) -> None:
        self._require_booted()
        self.kernel.run(max_ticks=max_ticks)

    def shutdown(self) -> None:
        self._require_booted()
        self.fs.unmount()
        self.machine = None
        self.device = None
        self.kernel = None
        self.fs = None
        self.shell = None

    def _require_booted(self) -> None:
        if self.kernel is None or self.fs is None:
            raise RuntimeError("MiniOS is not booted")

from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path


class FileBlockDevice:
    def __init__(self, path: Path, *, block_size: int, block_count: int) -> None:
        if block_size <= 0 or block_count <= 0:
            raise ValueError("block geometry must be positive")
        self.path = Path(path)
        self.block_size = block_size
        self.block_count = block_count
        self.total_size = block_size * block_count
        self.path.parent.mkdir(parents=True, exist_ok=True)
        if not self.path.exists():
            self.path.write_bytes(bytes(self.total_size))
        elif self.path.stat().st_size < self.total_size:
            with self.path.open("ab") as stream:
                stream.write(bytes(self.total_size - self.path.stat().st_size))

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

    def read_all(self) -> bytes:
        with self.path.open("rb") as stream:
            return stream.read(self.total_size)

    def write_all(self, data: bytes) -> None:
        if len(data) > self.total_size:
            raise OSError("device capacity exceeded")
        with self.path.open("r+b") as stream:
            stream.seek(0)
            stream.write(data)
            stream.write(bytes(self.total_size - len(data)))

    def _check(self, block: int) -> None:
        if not 0 <= block < self.block_count:
            raise IndexError(f"block out of range: {block}")


@dataclass(frozen=True)
class IORequest:
    request_id: int
    operation: str
    block: int
    data: bytes = b""

    @classmethod
    def write(cls, *, request_id: int, block: int, data: bytes) -> "IORequest":
        return cls(request_id=request_id, operation="write", block=block, data=data)

    @classmethod
    def read(cls, *, request_id: int, block: int) -> "IORequest":
        return cls(request_id=request_id, operation="read", block=block)


class SCANQueue:
    def __init__(self, *, head: int, direction: int) -> None:
        if head < 0:
            raise ValueError("head must be non-negative")
        if direction not in (-1, 1):
            raise ValueError("direction must be -1 or 1")
        self.head = head
        self.direction = direction
        self._requests: list[IORequest] = []

    def add(self, request: IORequest) -> None:
        self._requests.append(request)

    def drain(self) -> list[IORequest]:
        if self.direction == 1:
            forward = sorted((r for r in self._requests if r.block >= self.head), key=lambda r: r.block)
            reverse = sorted((r for r in self._requests if r.block < self.head), key=lambda r: r.block, reverse=True)
        else:
            forward = sorted((r for r in self._requests if r.block <= self.head), key=lambda r: r.block, reverse=True)
            reverse = sorted((r for r in self._requests if r.block > self.head), key=lambda r: r.block)
        ordered = forward + reverse
        if ordered:
            self.head = ordered[-1].block
        if reverse:
            self.direction *= -1
        self._requests = []
        return ordered


@dataclass(frozen=True)
class IOInterrupt:
    kind: str
    request_id: int


@dataclass
class IOTrace:
    block_order: list[int] = field(default_factory=list)
    interrupts: list[IOInterrupt] = field(default_factory=list)
    read_results: dict[int, bytes] = field(default_factory=dict)


class IOController:
    def __init__(self, *, device: FileBlockDevice, scheduler: SCANQueue) -> None:
        self.device = device
        self.scheduler = scheduler

    def submit(self, request: IORequest) -> None:
        self.device._check(request.block)
        self.scheduler.add(request)

    def drain(self) -> IOTrace:
        trace = IOTrace()
        for request in self.scheduler.drain():
            if request.operation == "write":
                self.device.write_block(request.block, request.data)
            elif request.operation == "read":
                trace.read_results[request.request_id] = self.device.read_block(request.block)
            else:
                raise ValueError(f"unknown I/O operation: {request.operation}")
            trace.block_order.append(request.block)
            trace.interrupts.append(IOInterrupt("IO_COMPLETE", request.request_id))
        return trace


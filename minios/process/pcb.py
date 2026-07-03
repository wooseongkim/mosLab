from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum, auto
from typing import Any


class ProcessState(Enum):
    NEW = auto()
    READY = auto()
    RUNNING = auto()
    BLOCKED = auto()
    TERMINATED = auto()


@dataclass
class PCB:
    pid: int
    program: list[tuple[Any, ...]]
    state: ProcessState = ProcessState.NEW
    pc: int = 0
    exit_code: int | None = None
    context: dict[str, Any] = field(default_factory=dict)


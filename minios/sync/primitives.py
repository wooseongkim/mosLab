from __future__ import annotations

from collections import defaultdict, deque
from dataclasses import dataclass


@dataclass(frozen=True)
class Message:
    sender: int
    receiver: int
    payload: bytes


class Mutex:
    def __init__(self) -> None:
        self.owner: int | None = None
        self.waiters: deque[int] = deque()

    def acquire(self, pid: int) -> bool:
        if self.owner is None:
            self.owner = pid
            return True
        if self.owner == pid:
            raise RuntimeError("mutex is not reentrant")
        if pid not in self.waiters:
            self.waiters.append(pid)
        return False

    def release(self, pid: int) -> int | None:
        if self.owner != pid:
            raise PermissionError("only the owner may release the mutex")
        self.owner = self.waiters.popleft() if self.waiters else None
        return self.owner


class SignalQueue:
    def __init__(self) -> None:
        self._queues: dict[int, deque[str]] = defaultdict(deque)

    def send(self, *, pid: int, signal: str) -> None:
        if not signal:
            raise ValueError("signal must not be empty")
        self._queues[pid].append(signal)

    def receive(self, *, pid: int) -> str | None:
        queue = self._queues[pid]
        return queue.popleft() if queue else None


class Mailbox:
    def __init__(self, *, capacity: int) -> None:
        if capacity <= 0:
            raise ValueError("mailbox capacity must be positive")
        self.capacity = capacity
        self._messages: deque[Message] = deque()

    def send(self, *, sender: int, receiver: int, payload: bytes) -> None:
        if len(self._messages) >= self.capacity:
            raise BufferError("mailbox is full")
        if not isinstance(payload, bytes):
            raise TypeError("payload must be bytes")
        self._messages.append(Message(sender, receiver, payload))

    def receive(self, *, receiver: int) -> Message:
        for index, message in enumerate(self._messages):
            if message.receiver == receiver:
                del self._messages[index]
                return message
        raise LookupError("mailbox has no message for receiver")


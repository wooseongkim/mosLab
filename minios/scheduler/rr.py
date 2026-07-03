from collections import deque


class RRScheduler:
    def __init__(self, *, quantum: int) -> None:
        if quantum <= 0:
            raise ValueError("quantum must be positive")
        self.quantum = quantum
        self._ready: deque[int] = deque()
        self._current: int | None = None
        self._used = 0

    def enqueue(self, pid: int) -> None:
        if pid == self._current or pid in self._ready:
            return
        self._ready.append(pid)

    def current_or_next(self) -> int:
        if self._current is None:
            if not self._ready:
                raise LookupError("ready queue is empty")
            self._current = self._ready.popleft()
            self._used = 0
        return self._current

    def on_tick(self, pid: int, *, finished: bool, blocked: bool) -> None:
        if self._current != pid:
            raise RuntimeError("tick does not belong to the running process")
        if finished or blocked:
            self._current = None
            self._used = 0
            return
        self._used += 1
        if self._used >= self.quantum:
            self._ready.append(pid)
            self._current = None
            self._used = 0

    def ready_pids(self) -> list[int]:
        result = list(self._ready)
        if self._current is not None:
            result.insert(0, self._current)
        return result


from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class Allocation:
    start: int
    size: int
    token: int


class ContiguousAllocator:
    def __init__(self, *, total_size: int, strategy: str) -> None:
        if total_size <= 0:
            raise ValueError("total size must be positive")
        if strategy != "first_fit":
            raise ValueError("only first_fit is required by this lab")
        self.total_size = total_size
        self.strategy = strategy
        self._free: list[tuple[int, int]] = [(0, total_size)]
        self._allocated: dict[int, Allocation] = {}
        self._next_token = 1

    def allocate(self, size: int) -> Allocation:
        if size <= 0:
            raise ValueError("allocation size must be positive")
        for index, (start, length) in enumerate(self._free):
            if length < size:
                continue
            allocation = Allocation(start, size, self._next_token)
            self._next_token += 1
            self._allocated[allocation.token] = allocation
            if length == size:
                del self._free[index]
            else:
                self._free[index] = (start + size, length - size)
            return allocation
        raise MemoryError("not enough contiguous memory")

    def free(self, allocation: Allocation) -> None:
        stored = self._allocated.get(allocation.token)
        if stored != allocation:
            raise ValueError("unknown or already freed allocation")
        del self._allocated[allocation.token]
        self._free.append((allocation.start, allocation.size))
        self._coalesce()

    def free_segments(self) -> list[tuple[int, int]]:
        return list(self._free)

    def check_invariants(self) -> list[str]:
        issues: list[str] = []
        segments = [(start, size, "free") for start, size in self._free]
        segments.extend((item.start, item.size, "allocated") for item in self._allocated.values())
        segments.sort()
        cursor = 0
        for start, size, kind in segments:
            if size <= 0:
                issues.append(f"{kind} segment has non-positive size")
            if start != cursor:
                issues.append(f"gap or overlap at {cursor}, next segment starts at {start}")
            cursor = max(cursor, start + size)
        if cursor != self.total_size:
            issues.append(f"segments end at {cursor}, expected {self.total_size}")
        return issues

    def _coalesce(self) -> None:
        merged: list[tuple[int, int]] = []
        for start, size in sorted(self._free):
            if merged and merged[-1][0] + merged[-1][1] == start:
                previous_start, previous_size = merged[-1]
                merged[-1] = (previous_start, previous_size + size)
            else:
                merged.append((start, size))
        self._free = merged


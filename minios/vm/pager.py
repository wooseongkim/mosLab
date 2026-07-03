from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path


PageKey = tuple[int, int]


@dataclass
class PagerStats:
    page_faults: int = 0
    swap_outs: int = 0
    swap_ins: int = 0


class Pager:
    def __init__(
        self,
        *,
        page_size: int,
        frame_count: int,
        policy: str,
        swap_path: Path,
    ) -> None:
        if page_size <= 0 or frame_count <= 0:
            raise ValueError("page size and frame count must be positive")
        if policy.upper() != "LRU":
            raise ValueError("this lab implementation supports LRU")
        self.page_size = page_size
        self.frame_count = frame_count
        self.policy = "LRU"
        self.swap_path = Path(swap_path)
        self.swap_path.parent.mkdir(parents=True, exist_ok=True)
        self.swap_path.write_bytes(b"")
        self.stats = PagerStats()
        self._frames: dict[PageKey, bytearray] = {}
        self._last_used: dict[PageKey, int] = {}
        self._swap_slots: dict[PageKey, int] = {}
        self._free_slots: list[int] = []
        self._next_slot = 0
        self._clock = 0

    def write(self, *, pid: int, virtual_address: int, data: bytes) -> None:
        if virtual_address < 0:
            raise ValueError("virtual address must be non-negative")
        if not isinstance(data, bytes):
            raise TypeError("data must be bytes")
        position = 0
        while position < len(data):
            address = virtual_address + position
            page = address // self.page_size
            offset = address % self.page_size
            chunk = min(self.page_size - offset, len(data) - position)
            frame = self._ensure_page((pid, page))
            frame[offset : offset + chunk] = data[position : position + chunk]
            position += chunk

    def read(self, *, pid: int, virtual_address: int, size: int) -> bytes:
        if virtual_address < 0 or size < 0:
            raise ValueError("address and size must be non-negative")
        output = bytearray()
        position = 0
        while position < size:
            address = virtual_address + position
            page = address // self.page_size
            offset = address % self.page_size
            chunk = min(self.page_size - offset, size - position)
            frame = self._ensure_page((pid, page))
            output.extend(frame[offset : offset + chunk])
            position += chunk
        return bytes(output)

    def is_swapped(self, *, pid: int, virtual_page: int) -> bool:
        return (pid, virtual_page) in self._swap_slots

    def _ensure_page(self, key: PageKey) -> bytearray:
        if key in self._frames:
            self._touch(key)
            return self._frames[key]
        self.stats.page_faults += 1
        if len(self._frames) >= self.frame_count:
            self._evict_lru()
        if key in self._swap_slots:
            slot = self._swap_slots.pop(key)
            page = bytearray(self._read_slot(slot))
            self._free_slots.append(slot)
            self.stats.swap_ins += 1
        else:
            page = bytearray(self.page_size)
        self._frames[key] = page
        self._touch(key)
        return page

    def _evict_lru(self) -> None:
        victim = min(self._frames, key=self._last_used.__getitem__)
        page = self._frames.pop(victim)
        del self._last_used[victim]
        slot = self._allocate_slot()
        self._write_slot(slot, page)
        self._swap_slots[victim] = slot
        self.stats.swap_outs += 1

    def _touch(self, key: PageKey) -> None:
        self._clock += 1
        self._last_used[key] = self._clock

    def _allocate_slot(self) -> int:
        if self._free_slots:
            return self._free_slots.pop()
        slot = self._next_slot
        self._next_slot += 1
        return slot

    def _write_slot(self, slot: int, data: bytes | bytearray) -> None:
        mode = "r+b" if self.swap_path.exists() else "w+b"
        with self.swap_path.open(mode) as stream:
            stream.seek(slot * self.page_size)
            stream.write(data)

    def _read_slot(self, slot: int) -> bytes:
        with self.swap_path.open("rb") as stream:
            stream.seek(slot * self.page_size)
            data = stream.read(self.page_size)
        if len(data) != self.page_size:
            raise OSError("short swap read")
        return data


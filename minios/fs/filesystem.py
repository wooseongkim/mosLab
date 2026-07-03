from __future__ import annotations

import base64
import json
from dataclasses import dataclass


MAGIC = b"MFS1"
HEADER_SIZE = 8


@dataclass
class OpenFile:
    path: str
    offset: int = 0


class FileSystem:
    def __init__(self, device, metadata: dict) -> None:
        self.device = device
        self.inode_count = int(metadata["inode_count"])
        self.files: dict[str, dict] = metadata["files"]
        self._fds: dict[int, OpenFile] = {}
        self._next_fd = 3

    @classmethod
    def format(cls, device, *, inode_count: int) -> None:
        if inode_count < 2:
            raise ValueError("inode count must be at least two")
        fs = cls(device, {"inode_count": inode_count, "files": {}})
        fs._save()

    @classmethod
    def mount(cls, device) -> "FileSystem":
        raw = device.read_all()
        if raw[:4] != MAGIC:
            raise OSError("invalid MiniOS file system magic")
        length = int.from_bytes(raw[4:8], "big")
        if length <= 0 or HEADER_SIZE + length > len(raw):
            raise OSError("corrupt MiniOS file system header")
        try:
            metadata = json.loads(raw[8 : 8 + length].decode("utf-8"))
        except (UnicodeDecodeError, json.JSONDecodeError) as exc:
            raise OSError("corrupt MiniOS file system metadata") from exc
        fs = cls(device, metadata)
        issues = fs.check_invariants()
        if issues:
            raise OSError("file system invariant failure: " + "; ".join(issues))
        return fs

    @classmethod
    def is_formatted(cls, device) -> bool:
        return device.read_all()[:4] == MAGIC

    def create(self, path: str) -> int:
        self._validate_path(path)
        if path in self.files:
            raise FileExistsError(path)
        used = {int(entry["inode"]) for entry in self.files.values()}
        inode = next((value for value in range(1, self.inode_count) if value not in used), None)
        if inode is None:
            raise OSError("no free inode")
        self.files[path] = {"inode": inode, "data": ""}
        self._save()
        return self.open(path)

    def open(self, path: str) -> int:
        if path not in self.files:
            raise FileNotFoundError(path)
        fd = self._next_fd
        self._next_fd += 1
        self._fds[fd] = OpenFile(path)
        return fd

    def read(self, fd: int, size: int) -> bytes:
        if size < 0:
            raise ValueError("read size must be non-negative")
        opened = self._open_file(fd)
        data = self._decode_data(self.files[opened.path]["data"])
        result = data[opened.offset : opened.offset + size]
        opened.offset += len(result)
        return result

    def write(self, fd: int, data: bytes) -> int:
        if not isinstance(data, bytes):
            raise TypeError("file data must be bytes")
        opened = self._open_file(fd)
        current = bytearray(self._decode_data(self.files[opened.path]["data"]))
        if opened.offset > len(current):
            current.extend(bytes(opened.offset - len(current)))
        end = opened.offset + len(data)
        if end > len(current):
            current.extend(bytes(end - len(current)))
        current[opened.offset:end] = data
        opened.offset = end
        self.files[opened.path]["data"] = base64.b64encode(current).decode("ascii")
        self._save()
        return len(data)

    def close(self, fd: int) -> None:
        if fd not in self._fds:
            raise ValueError(f"bad file descriptor: {fd}")
        del self._fds[fd]

    def unlink(self, path: str) -> None:
        if path not in self.files:
            raise FileNotFoundError(path)
        if any(opened.path == path for opened in self._fds.values()):
            raise OSError("file is open")
        del self.files[path]
        self._save()

    def unmount(self) -> None:
        if self._fds:
            raise OSError("cannot unmount with open files")
        self._save()

    def list_paths(self) -> list[str]:
        return sorted(self.files)

    def check_invariants(self) -> list[str]:
        issues: list[str] = []
        inodes: list[int] = []
        for path, entry in self.files.items():
            if not path.startswith("/") or path == "/":
                issues.append(f"invalid path: {path}")
            inode = int(entry.get("inode", -1))
            if not 1 <= inode < self.inode_count:
                issues.append(f"invalid inode for {path}: {inode}")
            inodes.append(inode)
            try:
                self._decode_data(entry.get("data", ""))
            except ValueError:
                issues.append(f"invalid data encoding for {path}")
        if len(inodes) != len(set(inodes)):
            issues.append("duplicate inode allocation")
        try:
            if self._encoded_size() > self.device.total_size:
                issues.append("metadata exceeds device capacity")
        except (TypeError, ValueError):
            issues.append("metadata cannot be encoded")
        return issues

    def inode_bitmap(self) -> list[bool]:
        bitmap = [False] * self.inode_count
        bitmap[0] = True
        for entry in self.files.values():
            bitmap[int(entry["inode"])] = True
        return bitmap

    def block_bitmap(self) -> list[bool]:
        bitmap = [False] * self.device.block_count
        bitmap[0] = True
        cursor = 1
        for path in sorted(self.files):
            size = len(self._decode_data(self.files[path]["data"]))
            blocks = (size + self.device.block_size - 1) // self.device.block_size
            for block in range(cursor, min(cursor + blocks, len(bitmap))):
                bitmap[block] = True
            cursor += blocks
        return bitmap

    def _save(self) -> None:
        metadata = {
            "inode_count": self.inode_count,
            "inode_bitmap": self.inode_bitmap(),
            "block_bitmap": self.block_bitmap(),
            "files": self.files,
        }
        payload = json.dumps(metadata, ensure_ascii=True, sort_keys=True, separators=(",", ":")).encode("utf-8")
        encoded = MAGIC + len(payload).to_bytes(4, "big") + payload
        self.device.write_all(encoded)

    def _encoded_size(self) -> int:
        payload = json.dumps(
            {"inode_count": self.inode_count, "files": self.files},
            ensure_ascii=True,
            sort_keys=True,
            separators=(",", ":"),
        ).encode("utf-8")
        return HEADER_SIZE + len(payload)

    def _open_file(self, fd: int) -> OpenFile:
        try:
            return self._fds[fd]
        except KeyError as exc:
            raise ValueError(f"bad file descriptor: {fd}") from exc

    @staticmethod
    def _decode_data(value: str) -> bytes:
        try:
            return base64.b64decode(value.encode("ascii"), validate=True)
        except Exception as exc:
            raise ValueError("invalid base64 file data") from exc

    @staticmethod
    def _validate_path(path: str) -> None:
        if not path.startswith("/") or path == "/" or "//" in path:
            raise ValueError(f"invalid absolute file path: {path}")


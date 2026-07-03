from __future__ import annotations

import struct
import zlib
from dataclasses import dataclass


MAGIC = b"MOS1"
HEADER = struct.Struct("!4sIIIII")


class PacketError(ValueError):
    pass


@dataclass(frozen=True)
class Packet:
    source: int
    destination: int
    sequence: int
    payload: bytes

    def encode(self) -> bytes:
        if min(self.source, self.destination, self.sequence) < 0:
            raise PacketError("packet identifiers must be non-negative")
        if not isinstance(self.payload, bytes):
            raise PacketError("payload must be bytes")
        checksum_input = struct.pack(
            "!4sIIII", MAGIC, self.source, self.destination, self.sequence, len(self.payload)
        ) + self.payload
        checksum = zlib.crc32(checksum_input) & 0xFFFFFFFF
        return HEADER.pack(
            MAGIC,
            self.source,
            self.destination,
            self.sequence,
            len(self.payload),
            checksum,
        ) + self.payload

    @classmethod
    def decode(cls, data: bytes) -> "Packet":
        if len(data) < HEADER.size:
            raise PacketError("packet is shorter than the header")
        magic, source, destination, sequence, length, checksum = HEADER.unpack(data[: HEADER.size])
        payload = data[HEADER.size :]
        if magic != MAGIC:
            raise PacketError("invalid packet magic")
        if len(payload) != length:
            raise PacketError("packet payload length mismatch")
        checksum_input = struct.pack("!4sIIII", magic, source, destination, sequence, length) + payload
        actual = zlib.crc32(checksum_input) & 0xFFFFFFFF
        if actual != checksum:
            raise PacketError("packet checksum mismatch")
        return cls(source, destination, sequence, payload)


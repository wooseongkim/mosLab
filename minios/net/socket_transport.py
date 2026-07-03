from __future__ import annotations

import socket
import struct


FRAME = struct.Struct("!I")


class SocketTransport:
    def __init__(self, *, listener=None, connection=None) -> None:
        self._listener = listener
        self._connection = connection

    @classmethod
    def listen(cls, host: str, port: int) -> "SocketTransport":
        listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        listener.bind((host, port))
        listener.listen(1)
        return cls(listener=listener)

    @classmethod
    def connect(cls, remote) -> "SocketTransport":
        return cls(connection=socket.create_connection(remote, timeout=2.0))

    @property
    def address(self):
        if self._listener is not None:
            return self._listener.getsockname()
        if self._connection is not None:
            return self._connection.getsockname()
        raise RuntimeError("transport is closed")

    def send_frame(self, payload: bytes) -> None:
        connection = self._require_connection(timeout=2.0)
        connection.sendall(FRAME.pack(len(payload)) + payload)

    def receive_frame(self, timeout: float) -> bytes:
        connection = self._require_connection(timeout=timeout)
        connection.settimeout(timeout)
        length = FRAME.unpack(self._receive_exact(connection, FRAME.size))[0]
        if length > 16 * 1024 * 1024:
            raise ValueError("network frame is too large")
        return self._receive_exact(connection, length)

    def close(self) -> None:
        if self._connection is not None:
            self._connection.close()
            self._connection = None
        if self._listener is not None:
            self._listener.close()
            self._listener = None

    def _require_connection(self, timeout: float):
        if self._connection is None and self._listener is not None:
            self._listener.settimeout(timeout)
            self._connection, _ = self._listener.accept()
        if self._connection is None:
            raise RuntimeError("transport has no connection")
        return self._connection

    @staticmethod
    def _receive_exact(connection, size: int) -> bytes:
        chunks = bytearray()
        while len(chunks) < size:
            chunk = connection.recv(size - len(chunks))
            if not chunk:
                raise ConnectionError("connection closed during frame")
            chunks.extend(chunk)
        return bytes(chunks)


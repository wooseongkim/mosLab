from __future__ import annotations

from minios.net.packet import Packet
from minios.net.socket_transport import SocketTransport


class MiniNode:
    def __init__(self, node_id: int, transport: SocketTransport) -> None:
        self.node_id = node_id
        self._transport = transport

    @classmethod
    def listen(cls, *, node_id: int, host: str, port: int) -> "MiniNode":
        return cls(node_id, SocketTransport.listen(host, port))

    @classmethod
    def connect(cls, *, node_id: int, remote) -> "MiniNode":
        return cls(node_id, SocketTransport.connect(remote))

    @property
    def address(self):
        return self._transport.address

    def send(self, packet: Packet) -> None:
        if packet.source != self.node_id:
            raise ValueError("packet source does not match node id")
        self._transport.send_frame(packet.encode())

    def receive(self, *, timeout: float) -> Packet:
        packet = Packet.decode(self._transport.receive_frame(timeout))
        if packet.destination != self.node_id:
            raise ValueError("packet destination does not match node id")
        return packet

    def close(self) -> None:
        self._transport.close()


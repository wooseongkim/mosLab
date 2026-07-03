from __future__ import annotations

import pytest


def machine(tmp_path, *, ram_size=16):
    from minios.hardware.machine import Machine

    return Machine(
        ram_size=ram_size,
        block_size=8,
        block_count=4,
        disk_path=tmp_path / "disk.img",
        timer_period=2,
    )


@pytest.mark.ch01
def test_hardware_rejects_out_of_range_access(tmp_path):
    target = machine(tmp_path)
    with pytest.raises(IndexError):
        target.memory.read8(16)
    with pytest.raises(IndexError):
        target.disk.read_block(4)
    with pytest.raises(ValueError):
        target.disk.write_block(0, b"short")


@pytest.mark.ch02
def test_syscall_is_rejected_before_boot(tmp_path):
    from minios.kernel.kernel import Kernel
    from minios.kernel.syscall import Syscall, SyscallError

    kernel = Kernel(machine(tmp_path))
    with pytest.raises(SyscallError):
        kernel.invoke(Syscall.CONSOLE_WRITE, b"x")


@pytest.mark.ch04
def test_mutex_and_mailbox_reject_invalid_operations():
    from minios.sync.primitives import Mailbox, Mutex

    mutex = Mutex()
    mutex.acquire(1)
    with pytest.raises(PermissionError):
        mutex.release(2)

    mailbox = Mailbox(capacity=1)
    mailbox.send(sender=1, receiver=2, payload=b"one")
    with pytest.raises(BufferError):
        mailbox.send(sender=1, receiver=2, payload=b"two")


@pytest.mark.ch06
def test_blocked_process_is_not_requeued():
    from minios.scheduler.rr import RRScheduler

    scheduler = RRScheduler(quantum=1)
    scheduler.enqueue(1)
    scheduler.enqueue(2)
    assert scheduler.current_or_next() == 1
    scheduler.on_tick(1, finished=False, blocked=True)
    assert scheduler.current_or_next() == 2
    assert 1 not in scheduler.ready_pids()


@pytest.mark.ch07
def test_allocator_detects_double_free():
    from minios.memory.allocator import ContiguousAllocator

    allocator = ContiguousAllocator(total_size=32, strategy="first_fit")
    allocation = allocator.allocate(8)
    allocator.free(allocation)
    with pytest.raises(ValueError):
        allocator.free(allocation)
    assert allocator.check_invariants() == []


@pytest.mark.ch08
def test_pager_isolates_processes_and_cross_page_access(tmp_path):
    from minios.vm.pager import Pager

    pager = Pager(page_size=4, frame_count=2, policy="LRU", swap_path=tmp_path / "swap.img")
    pager.write(pid=1, virtual_address=3, data=b"AB")
    pager.write(pid=2, virtual_address=3, data=b"XY")
    assert pager.read(pid=1, virtual_address=3, size=2) == b"AB"
    assert pager.read(pid=2, virtual_address=3, size=2) == b"XY"


@pytest.mark.ch09
def test_block_device_remount_does_not_truncate(tmp_path):
    from minios.io.block import FileBlockDevice

    path = tmp_path / "blocks.img"
    first = FileBlockDevice(path, block_size=8, block_count=4)
    first.write_block(3, b"PERSIST!")
    second = FileBlockDevice(path, block_size=8, block_count=4)
    assert second.read_block(3) == b"PERSIST!"


@pytest.mark.ch10
def test_file_system_rejects_duplicate_path(tmp_path):
    from minios.fs.filesystem import FileSystem
    from minios.io.block import FileBlockDevice

    device = FileBlockDevice(tmp_path / "fs.img", block_size=64, block_count=32)
    FileSystem.format(device, inode_count=8)
    fs = FileSystem.mount(device)
    fd = fs.create("/a")
    fs.close(fd)
    with pytest.raises(FileExistsError):
        fs.create("/a")
    assert sum(fs.inode_bitmap()) == 2
    assert fs.check_invariants() == []


@pytest.mark.ch11
def test_packet_checksum_detects_corruption():
    from minios.net.packet import Packet, PacketError

    encoded = bytearray(Packet(source=1, destination=2, sequence=3, payload=b"data").encode())
    encoded[-1] ^= 0xFF
    with pytest.raises(PacketError):
        Packet.decode(bytes(encoded))


@pytest.mark.ch13
def test_shell_overwrite_and_reboot(tmp_path):
    from minios.system import MiniOS

    path = tmp_path / "system.img"
    system = MiniOS(disk_path=path, ram_size=64)
    system.boot()
    system.shell.execute("echo long-value > /a")
    system.shell.execute("echo x > /a")
    assert system.shell.execute("cat /a").stdout == "x\n"
    system.shutdown()

    rebooted = MiniOS(disk_path=path, ram_size=64)
    rebooted.boot()
    assert rebooted.shell.execute("cat /a").stdout == "x\n"


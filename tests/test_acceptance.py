from __future__ import annotations

import pytest


def make_machine(tmp_path):
    from minios.hardware.machine import Machine

    return Machine(
        ram_size=64,
        block_size=8,
        block_count=8,
        disk_path=tmp_path / "disk.img",
        timer_period=2,
    )


@pytest.mark.ch01
def test_ch01_virtual_hardware(tmp_path):
    machine = make_machine(tmp_path)
    machine.load_program([
        ("MOVI", "R0", 7),
        ("STORE8", "R0", 10),
        ("HALT",),
    ])
    trace = machine.run(max_ticks=10)

    assert machine.memory.read8(10) == 7
    machine.disk.write_block(2, b"ABCDEFGH")
    assert machine.disk.read_block(2) == b"ABCDEFGH"
    assert trace.halted is True
    assert any(event.kind == "TIMER" for event in trace.interrupts)


@pytest.mark.ch02
def test_ch02_boot_and_syscall(tmp_path):
    from minios.kernel.kernel import Kernel
    from minios.kernel.syscall import Syscall, SyscallError

    kernel = Kernel(make_machine(tmp_path))
    kernel.boot()
    assert kernel.booted is True
    assert kernel.invoke(Syscall.CONSOLE_WRITE, b"OK") == 2
    assert kernel.console.output == b"OK"
    with pytest.raises(SyscallError):
        kernel.invoke(9999)


@pytest.mark.ch03
def test_ch03_process_lifecycle(tmp_path):
    from minios.kernel.kernel import Kernel
    from minios.process.pcb import ProcessState

    kernel = Kernel(make_machine(tmp_path))
    kernel.boot()
    first = kernel.spawn([("COMPUTE", 2), ("EXIT", 0)])
    second = kernel.spawn([("COMPUTE", 1), ("EXIT", 3)])
    kernel.run(max_ticks=20)

    assert kernel.processes[first].state is ProcessState.TERMINATED
    assert kernel.processes[second].state is ProcessState.TERMINATED
    assert kernel.wait(first) == 0
    assert kernel.wait(second) == 3
    assert kernel.trace.max_simultaneous_running <= 1


@pytest.mark.ch04
def test_ch04_mutex_signal_and_mailbox():
    from minios.sync.primitives import Mailbox, Mutex, SignalQueue

    mutex = Mutex()
    assert mutex.acquire(1) is True
    assert mutex.acquire(2) is False
    assert mutex.owner == 1
    assert list(mutex.waiters) == [2]
    assert mutex.release(1) == 2
    assert mutex.owner == 2

    signals = SignalQueue()
    signals.send(pid=2, signal="TERM")
    assert signals.receive(pid=2) == "TERM"

    mailbox = Mailbox(capacity=2)
    mailbox.send(sender=1, receiver=2, payload=b"hello")
    message = mailbox.receive(receiver=2)
    assert (message.sender, message.payload) == (1, b"hello")


@pytest.mark.ch06
def test_ch06_round_robin_quantum_two():
    from minios.scheduler.rr import RRScheduler

    scheduler = RRScheduler(quantum=2)
    for pid in (1, 2, 3):
        scheduler.enqueue(pid)

    sequence = []
    for _ in range(6):
        pid = scheduler.current_or_next()
        sequence.append(pid)
        scheduler.on_tick(pid, finished=False, blocked=False)

    assert sequence == [1, 1, 2, 2, 3, 3]
    assert scheduler.ready_pids() == [1, 2, 3]


@pytest.mark.ch07
def test_ch07_first_fit_and_coalescing():
    from minios.memory.allocator import ContiguousAllocator

    allocator = ContiguousAllocator(total_size=64, strategy="first_fit")
    first = allocator.allocate(16)
    second = allocator.allocate(8)
    assert (first.start, second.start) == (0, 16)

    allocator.free(first)
    allocator.free(second)
    assert allocator.free_segments() == [(0, 64)]
    assert allocator.check_invariants() == []


@pytest.mark.ch08
def test_ch08_lru_swap_preserves_data(tmp_path):
    from minios.vm.pager import Pager

    pager = Pager(page_size=4, frame_count=2, policy="LRU", swap_path=tmp_path / "swap.img")
    pager.write(pid=1, virtual_address=0, data=b"A")
    pager.write(pid=1, virtual_address=4, data=b"B")
    assert pager.read(pid=1, virtual_address=0, size=1) == b"A"
    pager.write(pid=1, virtual_address=8, data=b"C")

    assert pager.is_swapped(pid=1, virtual_page=1)
    assert pager.read(pid=1, virtual_address=4, size=1) == b"B"
    assert pager.stats.page_faults >= 4
    assert pager.stats.swap_outs >= 1
    assert pager.stats.swap_ins >= 1


@pytest.mark.ch09
def test_ch09_scan_and_io_completion(tmp_path):
    from minios.io.block import FileBlockDevice, IOController, IORequest, SCANQueue

    device = FileBlockDevice(tmp_path / "blocks.img", block_size=8, block_count=8)
    queue = SCANQueue(head=3, direction=1)
    controller = IOController(device=device, scheduler=queue)
    controller.submit(IORequest.write(request_id=1, block=1, data=b"11111111"))
    controller.submit(IORequest.write(request_id=2, block=7, data=b"77777777"))
    controller.submit(IORequest.write(request_id=3, block=4, data=b"44444444"))
    trace = controller.drain()

    assert trace.block_order == [4, 7, 1]
    assert device.read_block(4) == b"44444444"
    assert [event.kind for event in trace.interrupts] == ["IO_COMPLETE"] * 3


@pytest.mark.ch10
def test_ch10_file_system_persists_after_remount(tmp_path):
    from minios.fs.filesystem import FileSystem
    from minios.io.block import FileBlockDevice

    disk_path = tmp_path / "fs.img"
    device = FileBlockDevice(disk_path, block_size=32, block_count=64)
    FileSystem.format(device, inode_count=16)
    fs = FileSystem.mount(device)
    fd = fs.create("/hello.txt")
    fs.write(fd, b"hello MiniOS")
    fs.close(fd)
    fs.unmount()

    fs = FileSystem.mount(FileBlockDevice(disk_path, block_size=32, block_count=64))
    fd = fs.open("/hello.txt")
    assert fs.read(fd, 100) == b"hello MiniOS"
    fs.close(fd)
    fs.unlink("/hello.txt")
    with pytest.raises(FileNotFoundError):
        fs.open("/hello.txt")
    assert fs.check_invariants() == []


@pytest.mark.ch11
def test_ch11_packet_and_two_local_nodes():
    from minios.net.node import MiniNode
    from minios.net.packet import Packet

    server = MiniNode.listen(node_id=1, host="127.0.0.1", port=0)
    client = MiniNode.connect(node_id=2, remote=server.address)
    try:
        packet = Packet(source=2, destination=1, sequence=7, payload=b"ping")
        client.send(packet)
        received = server.receive(timeout=1.0)
        assert received == packet
    finally:
        client.close()
        server.close()


@pytest.mark.ch13
def test_ch13_shell_and_reboot_integration(tmp_path):
    from minios.system import MiniOS

    disk_path = tmp_path / "minios.img"
    system = MiniOS(disk_path=disk_path, ram_size=128)
    system.boot()
    assert system.shell.execute("echo hello > /note.txt").exit_code == 0
    assert system.shell.execute("cat /note.txt").stdout == "hello\n"
    result = system.shell.execute("run count 3")
    system.run(max_ticks=50)
    assert system.kernel.wait(result.pid) == 0
    system.shutdown()

    rebooted = MiniOS(disk_path=disk_path, ram_size=128)
    rebooted.boot()
    assert rebooted.shell.execute("cat /note.txt").stdout == "hello\n"
    assert rebooted.kernel.processes == {}


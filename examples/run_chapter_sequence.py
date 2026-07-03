from __future__ import annotations

from pathlib import Path
from tempfile import TemporaryDirectory

from minios.fs.filesystem import FileSystem
from minios.hardware.machine import Machine
from minios.io.block import FileBlockDevice, IOController, IORequest, SCANQueue
from minios.kernel.kernel import Kernel
from minios.kernel.syscall import Syscall
from minios.memory.allocator import ContiguousAllocator
from minios.net.node import MiniNode
from minios.net.packet import Packet
from minios.scheduler.rr import RRScheduler
from minios.sync.primitives import Mailbox, Mutex, SignalQueue
from minios.system import MiniOS
from minios.vm.pager import Pager


def main() -> None:
    with TemporaryDirectory(prefix="minios-sequence-") as directory:
        root = Path(directory)

        machine = Machine(
            ram_size=64,
            block_size=8,
            block_count=8,
            disk_path=root / "hardware.img",
            timer_period=2,
        )
        machine.load_program([("MOVI", "R0", 7), ("STORE8", "R0", 10), ("HALT",)])
        trace = machine.run(max_ticks=10)
        print(f"CH01 RAM[10]={machine.memory.read8(10)}, IRQs={[e.kind for e in trace.interrupts]}, halted={trace.halted}")

        kernel = Kernel(machine)
        kernel.boot()
        written = kernel.invoke(Syscall.CONSOLE_WRITE, b"OK")
        print(f"CH02 booted={kernel.booted}, console={kernel.console.output!r}, bytes={written}")

        first = kernel.spawn([("COMPUTE", 2), ("EXIT", 0)])
        second = kernel.spawn([("COMPUTE", 1), ("EXIT", 3)])
        kernel.run(max_ticks=20)
        print(f"CH03 pid={first}:{kernel.processes[first].state.name}/exit={kernel.wait(first)}, pid={second}:{kernel.processes[second].state.name}/exit={kernel.wait(second)}")

        mutex = Mutex()
        mutex.acquire(1)
        mutex.acquire(2)
        handed_to = mutex.release(1)
        signals = SignalQueue()
        signals.send(pid=2, signal="TERM")
        mailbox = Mailbox(capacity=2)
        mailbox.send(sender=1, receiver=2, payload=b"hello")
        message = mailbox.receive(receiver=2)
        print(f"CH04 mutex_owner={handed_to}, signal={signals.receive(pid=2)}, message={message.payload!r}")

        scheduler = RRScheduler(quantum=2)
        for pid in (1, 2, 3):
            scheduler.enqueue(pid)
        sequence = []
        for _ in range(6):
            pid = scheduler.current_or_next()
            sequence.append(pid)
            scheduler.on_tick(pid, finished=False, blocked=False)
        print(f"CH06 RR sequence={sequence}")

        allocator = ContiguousAllocator(total_size=64, strategy="first_fit")
        a = allocator.allocate(16)
        b = allocator.allocate(8)
        allocator.free(a)
        allocator.free(b)
        print(f"CH07 allocations=(0,16),(16,8), free={allocator.free_segments()}, invariants={allocator.check_invariants()}")

        pager = Pager(page_size=4, frame_count=2, policy="LRU", swap_path=root / "swap.img")
        pager.write(pid=1, virtual_address=0, data=b"A")
        pager.write(pid=1, virtual_address=4, data=b"B")
        pager.read(pid=1, virtual_address=0, size=1)
        pager.write(pid=1, virtual_address=8, data=b"C")
        swapped = pager.is_swapped(pid=1, virtual_page=1)
        restored = pager.read(pid=1, virtual_address=4, size=1)
        print(f"CH08 swapped_page1={swapped}, restored={restored!r}, faults={pager.stats.page_faults}, out={pager.stats.swap_outs}, in={pager.stats.swap_ins}")

        device = FileBlockDevice(root / "io.img", block_size=8, block_count=8)
        controller = IOController(device=device, scheduler=SCANQueue(head=3, direction=1))
        controller.submit(IORequest.write(request_id=1, block=1, data=b"11111111"))
        controller.submit(IORequest.write(request_id=2, block=7, data=b"77777777"))
        controller.submit(IORequest.write(request_id=3, block=4, data=b"44444444"))
        io_trace = controller.drain()
        print(f"CH09 block_order={io_trace.block_order}, IRQs={[e.kind for e in io_trace.interrupts]}")

        fs_path = root / "fs.img"
        fs_device = FileBlockDevice(fs_path, block_size=32, block_count=64)
        FileSystem.format(fs_device, inode_count=16)
        fs = FileSystem.mount(fs_device)
        fd = fs.create("/hello.txt")
        fs.write(fd, b"hello MiniOS")
        fs.close(fd)
        fs.unmount()
        fs = FileSystem.mount(FileBlockDevice(fs_path, block_size=32, block_count=64))
        fd = fs.open("/hello.txt")
        persisted = fs.read(fd, 100)
        fs.close(fd)
        print(f"CH10 remount_read={persisted!r}, inode_bitmap_used={sum(fs.inode_bitmap())}")

        server = MiniNode.listen(node_id=1, host="127.0.0.1", port=0)
        client = MiniNode.connect(node_id=2, remote=server.address)
        packet = Packet(source=2, destination=1, sequence=7, payload=b"ping")
        client.send(packet)
        received = server.receive(timeout=1.0)
        client.close()
        server.close()
        print(f"CH11 packet=src:{received.source}/dst:{received.destination}/seq:{received.sequence}/payload:{received.payload!r}")

        system_path = root / "system.img"
        system = MiniOS(disk_path=system_path, ram_size=128)
        system.boot()
        system.shell.execute("echo agentic-os > /hello.txt")
        process = system.shell.execute("run count 5")
        system.run(max_ticks=50)
        first_boot = system.shell.execute("cat /hello.txt").stdout.strip()
        exit_code = system.kernel.wait(process.pid)
        system.shutdown()
        rebooted = MiniOS(disk_path=system_path, ram_size=128)
        rebooted.boot()
        second_boot = rebooted.shell.execute("cat /hello.txt").stdout.strip()
        print(f"CH13 boot1_file={first_boot!r}, process_exit={exit_code}, boot2_file={second_boot!r}, reboot_processes={len(rebooted.kernel.processes)}")
        rebooted.shutdown()


if __name__ == "__main__":
    main()

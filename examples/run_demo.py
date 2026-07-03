from __future__ import annotations

from tempfile import TemporaryDirectory
from pathlib import Path

from minios.system import MiniOS


def main() -> None:
    with TemporaryDirectory(prefix="minios-demo-") as directory:
        disk = Path(directory) / "minios.img"
        system = MiniOS(disk_path=disk, ram_size=128)
        system.boot()
        system.shell.execute("echo agentic-os > /hello.txt")
        process = system.shell.execute("run count 5")
        system.run(max_ticks=50)
        print("boot-1 file:", system.shell.execute("cat /hello.txt").stdout.strip())
        print("boot-1 process:", process.pid, "exit", system.kernel.wait(process.pid))
        system.shutdown()

        rebooted = MiniOS(disk_path=disk, ram_size=128)
        rebooted.boot()
        print("boot-2 file:", rebooted.shell.execute("cat /hello.txt").stdout.strip())
        print("boot-2 process-count:", len(rebooted.kernel.processes))
        rebooted.shutdown()


if __name__ == "__main__":
    main()

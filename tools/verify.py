from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ORDER = ["ch01", "ch02", "ch03", "ch04", "ch06", "ch07", "ch08", "ch09", "ch10", "ch11", "ch13"]


def run(command: list[str]) -> int:
    print("+", " ".join(command))
    return subprocess.run(command, cwd=ROOT, check=False).returncode


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--chapter", required=True, choices=[value.upper() for value in ORDER])
    parser.add_argument("--hidden-tests", type=Path)
    args = parser.parse_args()

    chapter = args.chapter.lower()
    selected = ORDER[: ORDER.index(chapter) + 1]
    expression = " or ".join(selected)

    if run([sys.executable, "tools/check_forbidden.py"]) != 0:
        return 1

    command = [sys.executable, "-m", "pytest", "-q", "-m", expression, "tests"]
    if args.hidden_tests:
        command.append(str(args.hidden_tests.resolve()))
    return run(command)


if __name__ == "__main__":
    raise SystemExit(main())

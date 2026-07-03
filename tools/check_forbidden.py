from __future__ import annotations

import ast
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "minios"

BANNED_MODULES = {
    "asyncio",
    "ctypes",
    "fcntl",
    "mmap",
    "multiprocessing",
    "pickle",
    "posix_ipc",
    "queue",
    "shelve",
    "subprocess",
    "threading",
}

BANNED_CALLS = {
    "os.execv",
    "os.execve",
    "os.fork",
    "os.kill",
    "os.posix_spawn",
    "os.sched_yield",
    "os.system",
}


def dotted_name(node: ast.AST) -> str | None:
    if isinstance(node, ast.Name):
        return node.id
    if isinstance(node, ast.Attribute):
        parent = dotted_name(node.value)
        return f"{parent}.{node.attr}" if parent else node.attr
    return None


def inspect_file(path: Path) -> list[str]:
    try:
        tree = ast.parse(path.read_text(encoding="utf-8"), filename=str(path))
    except SyntaxError as exc:
        return [f"{path}:{exc.lineno}: syntax error: {exc.msg}"]

    issues: list[str] = []
    relative = path.relative_to(ROOT).as_posix()
    socket_allowed = relative == "minios/net/socket_transport.py"

    for node in ast.walk(tree):
        if isinstance(node, ast.Import):
            for alias in node.names:
                root = alias.name.split(".", 1)[0]
                if root in BANNED_MODULES:
                    issues.append(f"{relative}:{node.lineno}: forbidden import {alias.name}")
                if root == "socket" and not socket_allowed:
                    issues.append(f"{relative}:{node.lineno}: socket is allowed only in minios/net/socket_transport.py")
        elif isinstance(node, ast.ImportFrom) and node.module:
            root = node.module.split(".", 1)[0]
            if root in BANNED_MODULES:
                issues.append(f"{relative}:{node.lineno}: forbidden import {node.module}")
            if root == "socket" and not socket_allowed:
                issues.append(f"{relative}:{node.lineno}: socket is allowed only in minios/net/socket_transport.py")
        elif isinstance(node, ast.Call):
            name = dotted_name(node.func)
            if name in BANNED_CALLS:
                issues.append(f"{relative}:{node.lineno}: forbidden native call {name}")
    return issues


def main() -> int:
    if not SOURCE.exists():
        print(f"Missing student source directory: {SOURCE}", file=sys.stderr)
        return 2
    issues = [issue for path in SOURCE.rglob("*.py") for issue in inspect_file(path)]
    if issues:
        print("\n".join(issues), file=sys.stderr)
        return 1
    print("Forbidden API check passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())


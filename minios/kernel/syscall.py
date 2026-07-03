from enum import IntEnum


class Syscall(IntEnum):
    CONSOLE_WRITE = 1


class SyscallError(RuntimeError):
    pass


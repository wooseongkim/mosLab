#!/usr/bin/env sh
set -eu

echo "[LAB0] checking WSL/Linux environment"
if [ ! -r /proc/version ]; then
    echo "[FAIL] /proc/version is not readable"
    exit 1
fi

if grep -qi microsoft /proc/version; then
    echo "[PASS] running under WSL"
else
    echo "[WARN] this does not look like WSL; Linux builds may still work"
fi

for tool in gcc make ar; do
    if command -v "$tool" >/dev/null 2>&1; then
        echo "[PASS] found $tool"
    else
        echo "[FAIL] missing $tool"
        exit 1
    fi
done

echo "[PASS] LAB0 environment checks completed"

#!/bin/bash
cd "$(dirname "$0")"

echo "============================================================"
echo "             BREADTH-FIRST SEARCH - NHOM 3"
echo "============================================================"
echo

CXX="${CXX:-g++}"

if ! command -v "$CXX" >/dev/null 2>&1; then
    echo "[LOI] Khong tim thay g++."
    echo "Tren macOS, cai Xcode Command Line Tools:"
    echo "xcode-select --install"
    exit 1
fi

"$CXX" -std=c++11 bfs.cpp -o bfs

if [ $? -ne 0 ]; then
    echo
    echo "[LOI] Bien dich that bai."
    exit 1
fi

echo "[OK] Bien dich thanh cong."
echo
./bfs input.txt output.txt

#!/usr/bin/env bash
set -euo pipefail
SRC="main.cpp cipher.cpp"
OUT="main"
g++ -O2 $SRC -o "$OUT"
./"$OUT"

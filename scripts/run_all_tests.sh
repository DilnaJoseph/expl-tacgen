#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$DIR"

# Ensure all input files have clean LF line endings
sed -i 's/\r$//' tests/dilna/*.tac 2>/dev/null || true

echo "=========================================="
echo "    Running Dilna's Complete Test Suite   "
echo "=========================================="

# 1. Compile and run constant folding unit tests
echo "[1/4] Compiling and running fold_const unit tests..."
gcc -Wall -Wextra -g -Isrc src/fold.c src/stats.c tests/dilna/test_fold.c -o tests/dilna/test_fold
./tests/dilna/test_fold
rm -f tests/dilna/test_fold

# 2. While loop with break/continue
echo "[2/4] Testing while-loop with break/continue block splitting..."
./expltac tests/dilna/while_break.tac --tac -o tests/dilna
diff -u --strip-trailing-cr tests/dilna/while_break.tac tests/dilna/while_break.unopt.tac
echo "Pass: while_break.tac round-trip identical."

# 3. If-else ladder
echo "[3/4] Testing if-else branching block splitting..."
./expltac tests/dilna/if_else.tac --tac -o tests/dilna
diff -u --strip-trailing-cr tests/dilna/if_else.tac tests/dilna/if_else.unopt.tac
echo "Pass: if_else.tac round-trip identical."

# 4. Jump boundary
echo "[4/4] Testing jump to last instruction boundary..."
./expltac tests/dilna/jump_boundary.tac --tac -o tests/dilna
diff -u --strip-trailing-cr tests/dilna/jump_boundary.tac tests/dilna/jump_boundary.unopt.tac
echo "Pass: jump_boundary.tac round-trip identical."

echo "=========================================="
echo "     ALL TESTS PASSED SUCCESSFULLY!       "
echo "=========================================="

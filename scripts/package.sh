#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$DIR"

echo "Building clean checkout..."
make clean
make all

echo "Executing pipeline on sample..."
mkdir -p sample
if [ -f sample/sample.expl ]; then
    ./expltac sample/sample.expl -o sample
elif [ -f sample/sample.tac ]; then
    ./expltac sample/sample.tac --tac -o sample
fi

echo "Packaging repository into expl-tac.tar.gz..."
cd ..
tar --exclude='.git' --exclude='*.o' -czf expl-tac.tar.gz "$(basename "$DIR")"
mv expl-tac.tar.gz "$DIR/"

echo "Distribution package ready: $DIR/expl-tac.tar.gz"

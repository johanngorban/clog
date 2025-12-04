#!/bin/bash

set -e

PREFIX="/usr/local"

FILES="
$PREFIX/lib/libloggy.a
$PREFIX/include/loggy.h
$PREFIX/lib/cmake/loggy/loggyTargets.cmake
$PREFIX/lib/cmake/loggy/loggyTargets-noconfig.cmake
$PREFIX/lib/cmake/loggy/loggyConfig.cmake
$PREFIX/lib/cmake/loggy/loggyConfigVersion.cmake
"

echo "Removing loggy files..."

for f in $FILES; do
    if [ -f "$f" ]; then
        echo "  rm $f"
        rm -f "$f"
    else
        echo "  skip $f (not found)"
    fi
done

if [ -d "$PREFIX/lib/cmake/loggy" ]; then
    rmdir "$PREFIX/lib/cmake/loggy" 2>/dev/null || true
fi

echo "Done."

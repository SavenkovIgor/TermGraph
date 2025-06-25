#!/usr/bin/env bash

if command -v python3 >/dev/null 2>&1; then
    python3 "$(dirname "$0")/bootstrap.py" "$@"
else
    echo "python3 not found" >&2
    exit 1
fi

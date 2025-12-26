#!/usr/bin/env bash

cd "$(dirname "$0")"

CHECK_MODE=false
if [ "$1" = "--check" ]; then
  CHECK_MODE=true
fi

files=$(find . -type f \( -name "*.cpp" -o -name "*.h" \) \
  -not -path "*/node_modules/*" \
  -not -path "*/.bun/*" \
  -not -path "*/build/*" \
  -not -path "*/dist/*")

if [ -z "$files" ]; then
  echo "No C++ files found."
  exit 0
fi

if [ "$CHECK_MODE" = true ]; then
  echo "Checking C++ code style..."
  echo "$files" | xargs clang-format --dry-run --Werror
  if [ $? -eq 0 ]; then
    echo "All files are properly formatted!"
  else
    echo "Some files are not properly formatted. Run './format.sh' to fix."
    exit 1
  fi
else
  echo "Formatting C++ files..."
  echo "$files" | xargs clang-format -i
  echo "Done!"
fi

#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

OUT_DIR="tests/output"
mkdir -p "$OUT_DIR"

assert_file_exists() {
  if [[ ! -f "$1" ]]; then
    echo "Expected file does not exist: $1" >&2
    exit 1
  fi
}

assert_contains() {
  local output="$1"
  local expected="$2"
  if [[ "$output" != *"$expected"* ]]; then
    echo "Expected output to contain '$expected' but got: $output" >&2
    exit 1
  fi
}

# Echo
output=$(bin/pgmEcho tests/fixtures/ascii.pgm "$OUT_DIR/echo.pgm")
assert_contains "$output" "ECHOED"
assert_file_exists "$OUT_DIR/echo.pgm"

# Compare identical
output=$(bin/pgmComp tests/fixtures/ascii.pgm "$OUT_DIR/echo.pgm")
assert_contains "$output" "IDENTICAL"

# Compare different
output=$(bin/pgmComp tests/fixtures/ascii.pgm tests/fixtures/ascii_different.pgm)
assert_contains "$output" "DIFFERENT"

# Convert ASCII -> binary -> ASCII
output=$(bin/pgma2b tests/fixtures/ascii.pgm "$OUT_DIR/ascii_to_binary.pgm")
assert_contains "$output" "CONVERTED"
assert_file_exists "$OUT_DIR/ascii_to_binary.pgm"

output=$(bin/pgmb2a "$OUT_DIR/ascii_to_binary.pgm" "$OUT_DIR/binary_to_ascii.pgm")
assert_contains "$output" "CONVERTED"
assert_file_exists "$OUT_DIR/binary_to_ascii.pgm"

# Reduce
output=$(bin/pgmReduce tests/fixtures/ascii.pgm 2 "$OUT_DIR/reduced.pgm")
assert_contains "$output" "REDUCED"
assert_file_exists "$OUT_DIR/reduced.pgm"

# Tile
output=$(bin/pgmTile tests/fixtures/ascii.pgm 2 "$OUT_DIR/tile_<row>_<column>.pgm")
assert_contains "$output" "TILED"
assert_file_exists "$OUT_DIR/tile_0_0.pgm"
assert_file_exists "$OUT_DIR/tile_0_1.pgm"
assert_file_exists "$OUT_DIR/tile_1_0.pgm"
assert_file_exists "$OUT_DIR/tile_1_1.pgm"

# Invalid magic number should fail with exit code 3
set +e
bin/pgmEcho tests/fixtures/invalid_magic.pgm "$OUT_DIR/invalid_out.pgm" >/tmp/pgm_invalid_test.log 2>&1
status=$?
set -e
if [[ "$status" -ne 3 ]]; then
  echo "Expected invalid magic test to return 3, got $status" >&2
  cat /tmp/pgm_invalid_test.log >&2
  exit 1
fi

rm -f /tmp/pgm_invalid_test.log

echo "All tests passed."

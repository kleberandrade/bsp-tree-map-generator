#!/bin/sh
set -eu
cli=$1
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
"$cli" 20 10 5 2 42 > "$tmp/a"
"$cli" 20 10 5 2 42 > "$tmp/b"
cmp "$tmp/a" "$tmp/b"
test "$(wc -l < "$tmp/a" | tr -d ' ')" = 10
for args in '0 10 5 2 42' '20 10 33 2 42' '20 10 5 2 -1' '20 10 5 2 4294967296' 'oops' '20 10 5 2 1x'; do
    if "$cli" $args > /dev/null 2>&1; then exit 1; fi
done
printf 'CLI tests passed.\n'

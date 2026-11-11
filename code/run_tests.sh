#!/bin/sh
# Builds both programs and feeds them the same inputs I tried by hand.
cd "$(dirname "$0")"
gcc -Wall login.c -o login || exit 1
gcc -Wall login_fixed.c -o login_fixed || exit 1

A60=$(printf 'A%.0s' $(seq 1 60))
for input in secret hello AAAAAAAA AAAAAAAAA AAAAAAAAAAAA "$A60"; do
  echo "== login.c, input: $input (${#input} chars)"
  echo "$input" | ./login
  echo
done

for input in AAAAAAAAA AAAAAAAAAAAA; do
  echo "== login_fixed.c, input: $input"
  echo "$input" | ./login_fixed
  echo
done

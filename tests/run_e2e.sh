#!/bin/sh
# End-to-end checks for minitalk. Run from the repository root after `make all bonus`.
set -eu

MESSAGE='Hello, 42! مرحبا 👋'
tmp=$(mktemp -d)
server_pid=""

fail() {
    echo "FAIL: $*" >&2
    exit 1
}

stop_server() {
    if [ -n "$server_pid" ]; then
        kill "$server_pid" 2>/dev/null || true
        wait "$server_pid" 2>/dev/null || true
        server_pid=""
    fi
}

cleanup() {
    stop_server
    rm -rf "$tmp"
}
trap cleanup EXIT

# Start a server in the background and wait until it has printed its PID.
start_server() {
    : > "$tmp/out"
    "./$1" > "$tmp/out" &
    server_pid=$!
    tries=0
    until [ -s "$tmp/out" ]; do
        tries=$((tries + 1))
        [ "$tries" -le 50 ] || fail "$1 did not print its PID"
        sleep 0.1
    done
    sleep 0.1
    [ "$(head -n 1 "$tmp/out")" = "$server_pid" ] || fail "$1 printed the wrong PID"
}

# Everything the server printed after its PID line.
received() {
    tail -n +2 "$tmp/out"
}

echo "1. server prints ASCII, Arabic and emoji exactly"
start_server server
./client "$server_pid" "$MESSAGE"
sleep 0.3
got=$(received)
stop_server
[ "$got" = "$MESSAGE" ] || fail "server printed '$got', expected '$MESSAGE'"

echo "2. invalid PIDs are rejected before any signal is sent"
for bad in "" 0 -1 / abc 12a 99999999999; do
    status=0
    ./client "$bad" "x" 2>/dev/null || status=$?
    [ "$status" -eq 1 ] || fail "client accepted PID '$bad' (exit status $status)"
done

echo "3. a PID with no running process is reported"
status=0
# PIDs are always below pid_max, which is at most 4194304 on Linux.
./client 4194304 "x" 2>/dev/null || status=$?
[ "$status" -eq 1 ] || fail "client did not report a missing process (exit status $status)"

echo "4. the wrong number of arguments is rejected"
status=0
./client 2>/dev/null || status=$?
[ "$status" -eq 1 ] || fail "client without arguments exited with status $status"

echo "5. bonus server prints the message"
# There is no acknowledgement per bit, so on a busy machine two identical signals can
# merge and corrupt a message (see Limitations in the README). Allow three attempts.
attempt=1
while :; do
    start_server server_bonus
    ./client_bonus "$server_pid" "$MESSAGE" > /dev/null
    sleep 0.3
    got=$(received)
    stop_server
    [ "$got" = "$MESSAGE" ] && break
    attempt=$((attempt + 1))
    [ "$attempt" -le 3 ] || fail "server_bonus printed '$got', expected '$MESSAGE'"
done

echo "All end-to-end checks passed"

#!/bin/bash

# Test server

# Create directory folder to store result of tests
directory_path="/h/srolfe01/cs112/assignment2/tests"
mkdir -p "$directory_path"

# Remove results of prior test
rm -rf "${directory_path:?}"/*

# Launch proxy server
./server.out 9120 > "${directory_path}/server" 2>&1 & # Run server in background
server_pid=$!        # Capture process ID of server
sleep 1              # Wait for server to start

# Launch client server
./client.out 9120 Client1 > "${directory_path}/client1" &
client1_pid=$!       # Capture process ID of server
sleep 1              # Wait for server to start

./client.out 9120 Client2 > "${directory_path}/client2" &
client2_pid=$!       # Capture process ID of server
sleep 1              # Wait for server to start

# Stop proxy server
echo "Stopping server..."
kill $server_pid
kill $clint1_pid
kill $clint2_pid
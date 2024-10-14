#!/bin/bash

# Test server

# Create directory folder to store result of tests
directory_path="/h/srolfe01/cs112/assignment2/tests"
mkdir -p "$directory_path"

# Launch proxy server
./server.out 9120 & # Run proxy server in background
proxy_pid=$!        # Capture process ID of proxy
sleep 1             # Wait for proxy to start

# Remove results of prior test
rm -rf "${directory_path:?}"/*

# Launch client server
./client.out 9120 Client1 > "${directory_path}/client1" &
./client.out 9120 Client2 > "${directory_path}/client2" &

# Stop proxy server
echo "Stopping proxy server..."
kill $proxy_pid
#!/bin/bash

echo "Avvio HMI e le componenti"

binpath="./bin"

cd $binpath
./hmi_output &
./ecu &
./camera &
./steer &
./brake &
./throttle







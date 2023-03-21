#!/bin/bash
for i in `seq 1 1000`;
do
dd if=/dev/urandom | base64 | nc -v localhost 9999 &
#sleep 1
done

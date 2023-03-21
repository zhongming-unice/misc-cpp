#!/bin/bash
for i in `seq 1 10`;
do
echo $i | nc -v localhost 9999 &
#sleep 1
done

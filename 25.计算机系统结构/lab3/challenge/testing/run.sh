#!/bin/bash

testcase="test1"
debugger="challenge"

sudo sh -c "echo 0 > /proc/sys/kernel/randomize_va_space"

taskset -c 3 ./empty &

pid=$(ps x | grep empty | grep -v grep | awk '{print $1}')

sudo insmod ../${debugger}.ko

sudo mknod /dev/test c 511 0
sudo chmod 666 /dev/test

taskset -c 3 ./${testcase} &

sleep 5

./../analyse/debug > "output.log"

sudo rm /dev/test
sudo rmmod ${debugger}

kill ${pid}

echo "debugger exit"
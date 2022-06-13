#!/bin/bash

testcase="test"
debugger="task2"

sudo sh -c "echo 0 > /proc/sys/kernel/randomize_va_space"

taskset -c 3 ./empty &

pid=$(ps x | grep empty | grep -v grep | awk '{print $1}')

sudo insmod ../${debugger}.ko

taskset -c 3 ./${testcase} &

sleep 1

sudo rmmod ${debugger}

kill ${pid}

echo "debugger exit"
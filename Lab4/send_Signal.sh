#!/bin/bash
gcc -std=gnu99 -o drive MotorProgram.c
p8_path="$(find /sys/devices/ocp.3/ -name "pwm_test_P8_19*" -type d)"
p9_path="$(find /sys/devices/ocp.3/ -name  "pwm_test_P9*" -type d)"
a_path="$(find /sys/devices/ocp.3/ -name "helper*" -type d)"
./drive $a_path p8_path p9_path

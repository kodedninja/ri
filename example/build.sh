#!/bin/bash
cc -Os -Wall -Wextra -std=c89 -I ../src/ -I . -o example ../src/*.c ./*.c


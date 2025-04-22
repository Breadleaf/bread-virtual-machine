.PHONY: help dbg prod test

C_FLAGS := -Werror -Wall -Wpedantic -std=c11

all: help

help:
	@echo "help dbg prog test"

dbg:
	clang main.c -g $(C_FLAGS) -o vm

prog:
	clang main.c $(C_FLAGS) -O3 -o vm

test:
	clang main.c -g $(C_FLAGS) -o test -D BVM_TEST_MODE

clean:
	rm -rf vm vm.dSYM test test.dSYM

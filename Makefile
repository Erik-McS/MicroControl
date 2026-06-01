# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Files and directories
SRC = src/main.c src/alu.c src/cpu.c
TARGET = build/microcontrol

# Default target
all: $(TARGET)

# Compile the target
$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf build/*

test: test_alu test_cpu

test_alu: src/alu.c tests/test_alu.c	
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/test_alu src/alu.c tests/test_alu.c
	./build/test_alu

test_cpu: src/cpu.c src/alu.c tests/test_cpu.c	
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/test_cpu src/cpu.c src/alu.c tests/test_cpu.c
	./build/test_cpu

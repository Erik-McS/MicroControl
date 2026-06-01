# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Files and directories
SRC = src/main.c src/alu.c
TARGET = build/microcontrol

# Default target
all: $(TARGET)

# Compile the target
$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf build/*

test: src/alu.c tests/test_alu.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/test_alu src/alu.c tests/test_alu.c
	./build/test_alu
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

# Clean up build artifacts
clean:
	rm -rf build/*
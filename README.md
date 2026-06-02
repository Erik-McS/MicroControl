# MicroControl

A custom 8-bit CPU emulator and assembler built from scratch in C, as a practical exercise in computer architecture and low-level systems programming.

The project simulates a Von Neumann architecture machine with a working fetch-decode-execute cycle, a full ALU, a status flag register, and a two-tool chain: write assembly source, assemble it to a binary ROM, and run it on the emulated CPU.

---

## Architecture Overview

| Component | Detail |
| :--- | :--- |
| Memory | 256 bytes of RAM |
| Registers | Two general-purpose 8-bit registers (A and B) |
| Program Counter | 8-bit |
| Status Flags | Zero, Negative, Carry, Overflow |
| Data width | 8-bit |

### Status Flags

| Flag | Bit | Condition |
| :--- | :---: | :--- |
| Zero | 0 | Result equals 0 |
| Negative | 1 | Bit 7 of result is set (two's complement negative) |
| Carry | 2 | Unsigned overflow on addition |
| Overflow | 3 | Reserved for signed overflow detection |

---

## Instruction Set

| Mnemonic | Opcode | Bytes | Description |
| :--- | :---: | :---: | :--- |
| NOP | `0x00` | 1 | No operation |
| LDA | `0x01` | 2 | Load immediate value into Register A |
| LDB | `0x02` | 2 | Load immediate value into Register B |
| ADD | `0x03` | 1 | A = A + B. Updates Zero, Negative, Carry flags |
| SUB | `0x04` | 1 | A = A - B. Updates Zero and Negative flags |
| AND | `0x05` | 1 | A = A & B. Updates Zero and Negative flags |
| OR  | `0x06` | 1 | A = A \| B. Updates Zero and Negative flags |
| XOR | `0x07` | 1 | A = A ^ B. Updates Zero and Negative flags |
| INC | `0x08` | 1 | A = A + 1. Updates Zero and Negative flags |
| DEC | `0x09` | 1 | A = A - 1. Updates Zero and Negative flags |
| JMP | `0x0A` | 2 | Unconditional jump to address |
| OUT | `0x0B` | 1 | Print current value of Register A to stdout |
| STA | `0x0C` | 2 | Store Register A at a given RAM address |
| STB | `0x0D` | 2 | Store Register B at a given RAM address |
| JZ  | `0x0E` | 2 | Jump to address if Zero flag is set |
| HLT | `0xFF` | 1 | Halt execution |

---

## Toolchain

The project produces two separate binaries:

**Emulator** (`build/microcontrol`) — loads a raw binary ROM into virtual RAM and runs the CPU loop until `HLT`. In debug mode, prints a post-execution register and RAM dump showing the final machine state.

**Assembler** (`build/asm`) — takes a plain text assembly file and outputs a raw binary ready to load into the emulator. Supports inline comments with `#` and both decimal and hex operands.

---

## Project Structure

```
MicroControl/
├── src/
│   ├── main.c          # Emulator entry point and debug output
│   ├── cpu.c           # Fetch-decode-execute cycle
│   ├── alu.c           # Arithmetic Logic Unit and flag updates
│   └── asm.c           # Assembler entry point and parser
├── include/
│   ├── cpu.h           # CPU struct, ISA enum, flag definitions
│   ├── alu.h           # ALU operations enum and function declarations
│   └── asm.h           # Instruction struct and assembler declarations
├── tests/
│   ├── test_cpu.c      # Unit tests for CPU instructions
│   └── test_alu.c      # Unit tests for ALU operations
├── roms/               # Binary ROM files
├── Makefile
└── sonar-project.properties
```

---

## Building and Running

All build targets are managed through `make`.

**Build the emulator:**
```bash
make
```

**Build the assembler:**
```bash
make asm
```

**Run the test suite:**
```bash
make test
```

**Run a ROM:**
```bash
./build/microcontrol roms/loop.bin
```

---

## Writing and Assembling Programs

Assembly files are plain text, one instruction per line. Comments start with `#`.

```asm
# Countdown loop: decrement A until it reaches zero
LDA 0x05      # Load 5 into A
LDB 0x01      # Load 1 into B
SUB           # A = A - B, sets Zero flag when A reaches 0
JZ  0x00      # Jump back to start if zero
JMP 0x02      # Otherwise keep looping
HLT
```

Assemble and run:
```bash
./build/asm program.asm roms/program
./build/microcontrol roms/program.bin
```

---

## Roadmap

- JNZ (jump if not zero) for cleaner loop control
- JN (jump if negative) for signed branching
- Signed overflow detection for the Overflow flag
- Expanded test coverage for all ALU operations and new instructions
# MicroControl

A custom 8-bit CPU emulator and assembler built from scratch in C, as a practical exercise in computer architecture and low-level systems programming.

The project simulates a Von Neumann architecture machine with a working fetch-decode-execute cycle, an ALU, a status flag register, and a two-pass toolchain: write assembly, assemble it to a binary ROM, and run it on the emulated CPU.

---

## Architecture Overview

| Component | Detail |
| :--- | :--- |
| Memory | 256 bytes of RAM |
| Registers | Two general-purpose 8-bit registers (A and B) |
| Program Counter | 8-bit, wraps at 256 |
| Status Flags | 8-bit register (Zero flag implemented) |
| Data width | 8-bit |

---

## Instruction Set

| Mnemonic | Opcode | Bytes | Description |
| :--- | :---: | :---: | :--- |
| NOP | `0x00` | 1 | No operation |
| LDA | `0x01` | 2 | Load immediate value into Register A |
| LDB | `0x02` | 2 | Load immediate value into Register B |
| ADD | `0x03` | 1 | Add Register B to Register A, result stored in A. Sets Zero flag if result is 0 |
| JMP | `0x04` | 2 | Unconditional jump to address |
| OUT | `0x05` | 1 | Print current value of Register A to stdout |
| STA | `0x06` | 2 | Store Register A at a given RAM address |
| STB | `0x07` | 2 | Store Register B at a given RAM address |
| JZ  | `0x08` | 2 | Jump to address if Zero flag is set |
| HLT | `0xFF` | 1 | Halt execution |

---

## Toolchain

The project has two separate binaries:

**Emulator** (`build/microcontrol`) — loads a raw binary ROM file into virtual RAM and runs it through the CPU loop until a `HLT` instruction is encountered. In debug mode, prints a post-execution register and RAM dump.

**Assembler** (`build/asm`) — takes a plain text assembly file and outputs a raw binary ready to be loaded by the emulator. Supports inline comments with `#` and both decimal and hex operands (via `%i` format).

---

## Project Structure

```
MicroControl/
├── src/
│   ├── main.c          # Emulator entry point and debug output
│   ├── cpu.c           # Fetch-decode-execute cycle
│   ├── alu.c           # Arithmetic Logic Unit
│   └── asm.c           # Assembler entry point and parser
├── include/
│   ├── cpu.h           # CPU struct, ISA enum, hardware constants
│   ├── alu.h           # ALU operations enum and function declaration
│   └── asm.h           # Instruction struct and assembler function declarations
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

## Writing and Assembling a Program

Assembly files are plain text, one instruction per line. Comments start with `#`.

```asm
# Count down from 5 using register A
LDA 0x05      # Load 5 into A
LDB 0x01      # Load 1 into B
ADD           # A = A + B (with zero flag)
JZ 0x00       # Jump back to start if result is zero
JMP 0x02      # Otherwise loop
HLT
```

Assemble and run:
```bash
./build/asm program.asm roms/program
./build/microcontrol roms/program.bin
```

---

## Roadmap

- Carry flag for overflow detection on ADD
- SUB instruction
- JNZ (jump if not zero) for cleaner loop control
- Expanded ALU operations (AND, OR, XOR)

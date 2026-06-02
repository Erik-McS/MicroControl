# MicroControl

A custom 8-bit CPU emulator and assembler built from scratch in C, as a practical exercise in computer architecture and low-level systems programming.

The project simulates a RISC-inspired Von Neumann architecture machine with a fetch-decode-execute cycle, a register file, a full ALU, a status flag register, and a two-tool chain: write assembly source, assemble it to a binary ROM, and run it on the emulated CPU.

---

## Architecture Overview

| Component | Detail |
| :--- | :--- |
| Memory | 256 bytes of RAM |
| Register file | 4 general-purpose 8-bit registers (R0–R3) |
| Program Counter | 8-bit |
| Stack Pointer | 8-bit (reserved) |
| Status Flags | Zero, Negative, Carry, Overflow |
| Data width | 8-bit |

### Status Flags

| Flag | Bit | Condition |
| :--- | :---: | :--- |
| Zero | 0 | Result equals 0 |
| Negative | 1 | Bit 7 of result is set (two's complement negative) |
| Carry | 2 | Unsigned overflow on ADD, underflow on SUB |
| Overflow | 3 | Reserved for signed overflow detection |

---

## Routing Byte

Most instructions take a routing byte as their first operand. It encodes the addressing mode, destination register, and source register in a single byte:

```
[ I ] [ M ] [ x ] [ x ] [ Dest 1 ] [ Dest 0 ] [ Src 1 ] [ Src 0 ]
  7     6     5     4        3          2          1         0
```

| Bit | Name | Meaning |
| :---: | :--- | :--- |
| 7 | Immediate (I) | Operand follows in the next byte, or selects immediate/register-indirect mode |
| 6 | Memory (M) | Operation involves a memory address |
| 3–2 | Destination | Index into the register file (0–3) |
| 1–0 | Source | Index into the register file (0–3) |

### Addressing Modes

| I | M | Mode | Description |
| :---: | :---: | :--- | :--- |
| 0 | 0 | Register to Register | Source and destination are both registers |
| 1 | 0 | Immediate | Value follows in the next byte |
| 0 | 1 | Memory Direct | Address follows in the next byte |
| 1 | 1 | Register Indirect | Address is held in the source register |

---

## Instruction Set

| Mnemonic | Opcode | Description |
| :--- | :---: | :--- |
| NOP | `0x00` | No operation |
| LD  | `0x01` | Load value into destination register. Supports immediate, memory direct, register indirect, and register-to-register modes |
| ADD | `0x02` | Destination = Destination + Source. Supports all addressing modes. Updates Zero, Negative, Carry flags |
| SUB | `0x03` | Destination = Destination - Source. Supports all addressing modes. Updates Zero, Negative, Carry flags |
| AND | `0x04` | Destination = Destination & Source. Supports all addressing modes. Updates Zero and Negative flags |
| OR  | `0x05` | Destination = Destination \| Source. Supports all addressing modes. Updates Zero and Negative flags |
| XOR | `0x06` | Destination = Destination ^ Source. Supports all addressing modes. Updates Zero and Negative flags |
| INC | `0x07` | Destination = Destination + 1. Updates Zero and Negative flags |
| DEC | `0x08` | Destination = Destination - 1. Updates Zero and Negative flags |
| JMP | `0x09` | Jump to address. Supports immediate mode (address in next byte) and register indirect (address in source register) |
| OUT | `0x0A` | Print value of destination register to stdout |
| ST  | `0x0B` | Store source register to memory. Supports memory direct and register indirect modes |
| JZ  | `0x0C` | Jump if Zero flag is set. Supports immediate and register indirect modes |
| HLT | `0xFF` | Halt execution |

---

## Toolchain

The project produces two separate binaries:

**Emulator** (`build/microcontrol`) — loads a raw binary ROM into virtual RAM and runs the CPU loop until `HLT` or an illegal instruction. In debug mode, prints a post-execution register and RAM dump showing the final machine state.

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
│   ├── cpu.h           # CPU struct, ISA enum, routing macros, flag definitions
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
# Load 5 into R0, decrement until zero
LD  0x80 0x05   # Immediate load: R0 = 5  (routing: I=1, M=0, dest=R0)
DEC 0x00        # Decrement R0            (routing: dest=R0)
JZ  0x80 0x00   # Jump to 0x00 if zero    (routing: immediate mode)
JMP 0x80 0x01   # Jump back to DEC
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
- Stack operations (PUSH / POP) using the stack pointer
- Expanded test coverage for the new routing byte addressing modes
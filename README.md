# MicroControl

A custom 8-bit Virtual Machine and CPU Emulator built entirely from scratch in standard C. 

This project simulates a basic Von Neumann architecture, featuring a modular Fetch-Decode-Execute cycle, an Arithmetic Logic Unit (ALU), and dynamic binary file I/O for loading custom "cartridges" (ROMs) into virtual memory.

--- This is a samll project to revise on my C and CA knowledge. I am fully aware that i reinvent the wheel here. ---

## ⚙️ Hardware Specifications
* **Memory:** 256 bytes of virtual RAM.
* **Registers:** Two general-purpose 8-bit registers (A and B).
* **Program Counter (PC):** Tracks the current execution address.
* **Architecture:** 8-bit instruction and data width.

## 🧠 Instruction Set Architecture (ISA)
MicroControl uses a custom, minimal assembly language. The current working opcodes are:

| Mnemonic | Hex | Bytes | Description |
| :--- | :---: | :---: | :--- |
| **NOP** | `0x00` | 1 | No Operation. Skips to the next instruction. |
| **LDA** | `0x01` | 2 | Load Register A. Loads the *next* byte in memory into Register A. |
| **LDB** | `0x02` | 2 | Load Register B. Loads the *next* byte in memory into Register B. |
| **ADD** | `0x03` | 1 | Add. Adds Register B to Register A and stores the result in Register A. |
| **HLT** | `0xFF` | 1 | Halt. Safely terminates CPU execution. |

## 🗺️ Roadmap / Up Next
* **Control Flow:** Implement Unconditional Jumps (`JMP`) to allow for infinite loops and Turing completeness.
* **I/O Operations:** Implement an Output instruction (`OUT`) for real-time terminal monitoring.

## 📂 Project Structure
```text
MicroControl/
├── src/            # Source code (main.c, CPU logic)
├── include/        # Header files (cpu.h hardware definitions)
├── build/          # Ephemeral compiled binaries
├── roms/           # Raw binary (.bin) cartridge files
├── sonar-project.properties  # SonarCloud static analysis config
└── README.md
```

## 🚀 Getting Started

### 1. Compilation
The project compiles cleanly under GCC with strict warning flags (`-Wall -Wextra`). To build the emulator from the root directory:

```bash
gcc -Wall -Wextra -Iinclude src/main.c -o build/microcontrol
```
*(Optional: Add `-DDEBUG` to enable memory hex-dumping during execution).*

### 2. Running a Program
MicroControl expects a raw binary file passed as a command-line argument. We store these virtual "cartridges" in the `roms/` directory.

```bash
./build/microcontrol roms/your_program.bin
```

### 3. Example Cartridge
You can generate a test ROM directly from the terminal using `printf` to test the ALU (Addition: 16 + 32 = 48):

```bash
printf '\x01\x10\x02\x20\x03\xFF' > roms/math.bin
./build/microcontrol roms/math.bin
```
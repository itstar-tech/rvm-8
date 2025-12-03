# RVM-8 Specifications

## 1. Overview

RVM-8 is a custom-designed 8-bit virtual console featuring a simple CPU, a minimal graphics processing unit (PPU), an assembler, and a ROM format. Its goal is to provide a retro-style, low-level programming experience while maintaining clarity, portability, and educational value.

## 2. CPU Architecture

* **8-bit registers:** A, X, Y
* **16-bit registers:** PC (Program Counter), SP (Stack Pointer)
* **FLAGS:** Zero, Carry, Negative, Overflow
* **Address space:** 64 KB
* **Instruction cycle:** Fetch → Decode → Execute
* **Instruction set categories:**

    * Data Movement (LDA, STA, TAX, etc.)
    * Arithmetic & Logic (ADD, SUB, AND, ORA, XOR)
    * Control Flow (JMP, JSR, RTS, BEQ, BNE, BCC)
    * Memory Access (LDX, LDY, STX, STY)
    * Stack operations (PHA, PLA, PHP, PLP)

## 3. Memory Map

| Address Range | Description               |
| ------------- | ------------------------- |
| 0x0000–0x1FFF | RAM                       |
| 0x2000–0x23FF | VRAM (Tile + Sprite Data) |
| 0x2400–0x24FF | PPU Registers             |
| 0x2500–0x250F | Input Registers           |
| 0xFF00–0xFFFF | ROM (program code)        |

> A diagram could be added later to visualize the memory layout more intuitively.

## 4. PPU (Pixel Processing Unit)

* **Resolution:** 160×144 pixels
* **Tiles:** 8×8 pixels
* **Color model:** 4-color grayscale palette
* **Background rendering:** Tilemap-based
* **Sprites:** Up to 16 on screen; simple priority ordering
* **Rendering pipeline:**

    1. Build tilemap frame
    2. Render background
    3. Overlay sprites
    4. Output framebuffer

## 5. Input System

The virtual console exposes simple gamepad input via memory-mapped registers:

* **Directional:** Up, Down, Left, Right
* **Buttons:** A, B, Start, Select

Games read these registers once per frame during the main loop.

## 6. ROM Format (.rvm)

A custom binary format containing:

1. Header (magic number, version, metadata)
2. Code section
3. Data section
4. Optional asset blocks (tile data, palettes)

## 7. Assembler

The assembler takes `.asm` files and outputs `.rvm` ROMs.
Features:

* Labels
* Pseudoinstructions
* Macros (optional in later versions)
* Data directives: `.db`, `.dw`, `.byte`, `.word`

**Example:**

```
LDA #10
STA $2000
JMP loop
```

## 8. Emulator

The emulator is responsible for running ROMs by simulating CPU, memory, and PPU behavior.

* Executes one instruction at a time
* Handles interrupts (future feature)
* Refreshes display at 60 FPS
* Reads input each frame

## 9. Sample Game

The official demo game includes:

* A playable character
* Collision with simple obstacles
* A collectible item
* “YOU WIN” screen

## 10. Debugger (Optional Feature)

Tools for introspection during development:

* Register view
* Memory inspector
* Step-by-step execution
* Breakpoints

## 11. Project Goals

* Educational clarity
* Simplicity over accuracy
* Reasonable performance (native + WebAssembly)
* Clean and well-documented architecture

## 12. Future Extensions

* Audio chip emulation
* More instructions
* Color palette expansion
* Tile editor

---


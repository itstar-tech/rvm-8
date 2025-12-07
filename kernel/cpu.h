/**
 * rvm-8/kernel/cpu.h
 *
 * CPU abstraction for the rvm-8 emulator.
 *
 * Copyright (c) 2025 foxomax
 * SPDX-License-Identifier: MIT
 *
 * This header declares the main `CPU` structure and the public
 * functions used to initialize, reset and step the CPU, as well
 * as memory access helpers.
 */

#ifndef RVM_CPU_H
#define RVM_CPU_H

#include <stdint.h>

#define RVM_MEM_SIZE 65536

/**
 * CPU register conventions and notes (inspired by the MOS 6502):
 *
 * - PC (Program Counter): 16-bit. Addresses the 64 KiB memory map
 *   (range 0x0000..0xFFFF). The PC must be wide enough to reference
 *   any address in the 16-bit address space.
 *
 * - X, Y (Index registers): 8-bit. Used as loop counters and for
 *   indexed addressing modes. X and Y behave similarly but are used
 *   by different addressing modes.
 *
 * - A (Accumulator): 8-bit. The primary register for arithmetic and
 *   logic; most operations read from or write to the accumulator.
 *
 * - SP (Stack Pointer): 16-bit. The stack grows downward: a push
 *   typically decrements SP and a pop increments it.
 *
 * - Flags: 8-bit processor status containing condition flags that
 *   reflect the result of the most recent operations.
 *
 * - memory: pointer to the CPU's RAM backing store (byte array of
 *   size RVM_MEM_SIZE).
 */
/**
 * @brief Core CPU state for the rvm-8 emulator.
 *
 * This struct contains the minimal set of registers and a pointer to
 * the backing memory required to emulate a simple 8-bit CPU with a
 * 16-bit address space.
 */
typedef struct {
  /** Accumulator (8-bit) */
  uint8_t a;
  /** X index register (8-bit) */
  uint8_t x;
  /** Y index register (8-bit) */
  uint8_t y;
  /** Program counter (16-bit) */
  uint16_t pc;
  /** Stack pointer (16-bit) — stack grows downward */
  uint16_t sp;
  /** Processor status / flags (8-bit) */
  uint8_t flags;
  /** Pointer to the RAM backing store (RVM_MEM_SIZE bytes) */
  uint8_t *memory;
  /** Total Cycles*/
  uint32_t cycles;
} CPU;

/**
 * @brief Defines the addressing modes for CPU instructions.
 *
 * Addressing modes determine how the CPU fetches operands for instructions.
 * Each mode has different performance characteristics and use cases.
 */
typedef enum {
  MODE_IMMEDIATE,   // LDA #10
  MODE_ZEROPAGE,    // LDA $00
  MODE_ABSOLUTE,    // LDA $1234
  MODE_ZEROPAGE_X,  // LDA $10,X
  MODE_ZEROPAGE_Y,  // LDA $10,X
  MODE_ABSOLUTE_X,  // LDA $2000,X
  MODE_ABSOLUTE_Y,  // LDA $2000,Y
  MODE_INDIRECT,    // JMP ($1234)
  MODE_INDIRECT_X,  // LDA ($10,X)
  MODE_INDIRECT_Y,  // LDA ($10),Y
  MODE_IMPLIED,     // CLC, NOP, INX
  MODE_ACCUMULATOR, // ASL A
  MODE_RELATIVE     // BNE, BEQ
} AddressingMode;

/**
 * @brief Function pointer for an instruction handler.
 *
 * Each instruction in the CPU is implemented by a handler function that
 * takes a pointer to the CPU state and an addressing mode, and returns
 * the number of cycles it consumed.
 */
typedef uint8_t (*InstructionHandler)(CPU *cpu, AddressingMode mode);

/**
 * @brief Represents a single CPU instruction.
 *
 * This struct defines an instruction's mnemonic name, its handler function,
 * the addressing mode it uses, and its base cycle count.
 */
typedef struct {
  const char *name;
  InstructionHandler handler;
  AddressingMode mode;
  uint8_t cycles;
} Instruction;

extern Instruction instruction_table[256];

uint8_t mem_read(CPU *cpu, uint16_t addr);
void mem_write(CPU *cpu, uint16_t addr, uint8_t val);

/*
 *
 *  Flags ------------
 *  Carry        = 0000001
 *  Zero         = 0000010
 *  Interruption = 0000100
 *  Decimal      = 0001000
 *  Break        = 0010000
 *  Overflow     = 0100000
 *  Negative     = 1000000
 *  ------------------
 */
enum FLAGS_BITS {
  FLAG_C = 1 << 0,
  FLAG_Z = 1 << 1,
  FLAG_I = 1 << 2,
  FLAG_D = 1 << 3,
  FLAG_B = 1 << 4,
  FLAG_V = 1 << 6,
  FLAG_N = 1 << 7,
};

/**
 * @brief Initialize a CPU instance.
 *
 * Sets up register defaults and attaches the provided memory buffer.
 *
 * @param cpu Pointer to an allocated CPU structure to initialize.
 * @param memory Pointer to a byte buffer of at least RVM_MEM_SIZE bytes.
 */
void cpu_init(CPU *cpu, uint8_t *memory);

/**
 * @brief Reset CPU registers to their power-on defaults.
 *
 * This does not change the memory pointer; use cpu_init to attach
 * memory if needed.
 *
 * @param cpu Pointer to the CPU instance to reset.
 */
void cpu_reset(CPU *cpu);

/**
 * @brief Execute one CPU instruction (single step).
 *
 * Advances the PC and updates registers/flags according to the
 * semantics of the executed opcode.
 *
 * @param cpu Pointer to the CPU instance to step.
 */
void cpu_step(CPU *cpu);

/**
 * @brief Read a byte from the CPU memory.
 *
 * This helper centralizes memory reads and can be used to add bounds
 * checks, memory-mapped I/O handling, or instrumentation later.
 *
 * @param cpu Pointer to the CPU instance.
 * @param addr 16-bit memory address to read from.
 * @return The byte read from memory.
 */
uint8_t mem_read(CPU *cpu, uint16_t addr);

/**
 * @brief Write a byte to the CPU memory.
 *
 * This helper centralizes memory writes and can be used to implement
 * memory-mapped I/O or write protection in the future.
 *
 * @param cpu Pointer to the CPU instance.
 * @param addr 16-bit memory address to write to.
 * @param val Byte value to write.
 */
void mem_write(CPU *cpu, uint16_t addr, uint8_t val);

void init_instruction_table(void);

static inline uint8_t lo8(int value) { return value & 0xFF; }

#endif

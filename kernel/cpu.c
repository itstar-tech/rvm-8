/*
 * rvm-8/kernel/cpu.c
 *
 * Partial implementation of the rvm-8 emulator CPU.
 *
 * Copyright (c) 2025 foxomax
 * SPDX-License-Identifier: MIT
 *
 * Notes:
 * - This file contains flag definitions and CPU state-related
 *   functionality. Implementation will be extended with instructions
 *   and more logic over time.
 */

#include "cpu.h"
#include <string.h>
#include <stdio.h>


uint8_t mem_read(CPU *cpu, uint16_t addr) {
    return cpu->memory[addr];
}

/**
 * @brief Writes a byte to a specified memory address.
 *
 * This function writes the given value to the memory at the specified address.
 *
 * @param cpu Pointer to the CPU instance.
 * @param addr The 16-bit memory address to write to.
 * @param val The 8-bit value to write.
 */
void mem_write(CPU *cpu, uint16_t addr, uint8_t val) {
    cpu->memory[addr] = val;
}


void cpu_init(CPU *cpu, uint8_t *memory) {
    uint8_t *mem = memory;

    init_instruction_table();

    memset(cpu, 0, sizeof(CPU));
    cpu->memory = mem;
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0xFD;
    cpu->flags = FLAG_I;

    uint16_t lo = cpu->memory[0xFFFC];
    uint16_t hi = cpu->memory[0xFFFD];

    cpu->pc = (hi << 8) | lo;
    cpu->cycles = 0;
}

void cpu_reset(CPU *cpu) {
    uint16_t lo = cpu->memory[0xFFFC];
    uint16_t hi = cpu->memory[0xFFFD];

    cpu->pc = (hi << 8) | lo;
    cpu->flags = FLAG_I;
    cpu->sp = 0xFD;
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->cycles = 0;
}

/**
 * @brief Executes a single CPU instruction.
 *
 * This function fetches an opcode from memory, looks up the corresponding
 * instruction, and executes its handler. It also handles illegal opcodes.
 *
 * @param cpu Pointer to the CPU instance.
 */
void cpu_step(CPU *cpu) {
    uint8_t opcode = mem_read(cpu, cpu->pc++);
    Instruction instr = instruction_table[opcode];

    if(instr.handler == NULL) {
        printf("Illegal opcode 0x%02X at PC 0x%04X\n", opcode, cpu->pc-1);
        return;
    }

    uint8_t cycles = instr.handler(cpu, instr.mode);
    cpu->cycles += cycles;
}
/*
 * rvm-8/kernel/opcodes.c
 *
 * Implementation of opcode tables/handlers for rvm-8 (currently a stub).
 *
 * Copyright (c) 2025 foxomax
 * SPDX-License-Identifier: MIT
 */
#include "cpu.h"
#include <string.h>
#include <stdio.h>

Instruction instruction_table[256];

/**
 * @brief Reads the address for the immediate addressing mode.
 *
 * In immediate addressing, the operand is the byte immediately following the
 * opcode. This function returns the current program counter and increments it.
 *
 * @param cpu Pointer to the CPU instance.
 * @return The address of the immediate value.
 */
uint16_t addr_immediate(CPU *cpu) {
    return cpu->pc++;
}


/**
 * @brief Reads the address for the zero-page addressing mode.
 *
 * In zero-page addressing, the operand is an 8-bit address in the first
 * 256 bytes of memory (0x0000-0x00FF). This function reads the address
 * from the current program counter.
 *
 * @param cpu Pointer to the CPU instance.
 * @return The 8-bit zero-page address.
 */
uint16_t addr_zeropage(CPU *cpu) {
    return mem_read(cpu, cpu->pc++);
}


/**
 * @brief Reads the address for the absolute addressing mode.
 *
 * In absolute addressing, the operand is a full 16-bit address. This
 * function reads the two bytes following the opcode to form the address.
 *
 * @param cpu Pointer to the CPU instance.
 * @return The 16-bit absolute address.
 */
uint16_t addr_absolute(CPU *cpu) {
    uint16_t lo = mem_read(cpu, cpu->pc++);
    uint16_t hi = mem_read(cpu, cpu->pc++);
    return (hi << 8) | lo;
}


/**
 * @brief Handles the LDA (Load Accumulator) instruction.
 *
 * This function loads a byte from memory into the accumulator, updating the
 * Zero (Z) and Negative (N) flags based on the value loaded.
 *
 * @param cpu Pointer to the CPU instance.
 * @param mode The addressing mode used by the instruction.
 * @return The number of cycles consumed by the instruction.
 */
uint8_t handler_lda(CPU *cpu, AddressingMode mode) {
    uint16_t addr;
    uint8_t value;

    switch(mode) {
        case MODE_IMMEDIATE:
            addr = addr_immediate(cpu);
            value = mem_read(cpu, addr);
            break;
        case MODE_ZEROPAGE:
            addr = addr_zeropage(cpu);
            value = mem_read(cpu, addr);
            break;
        case MODE_ABSOLUTE:
            addr = addr_absolute(cpu);
            value = mem_read(cpu, addr);
            break;
        default:
            printf("Unimplemented addressing mode\n");
            return 0;
    }

    cpu->a = value;

    cpu->flags &= ~(FLAG_Z | FLAG_N);
    if (cpu->a == 0) cpu->flags |= FLAG_Z;
    if (cpu->a & 0x80) cpu->flags |= FLAG_N;

    return 2;
}


/**
 * @brief Handles the ADC (Add with Carry) instruction.
 *
 * This function adds a value from memory and the carry flag to the accumulator.
 * It updates the Zero (Z), Negative (N), Overflow (V), and Carry (C) flags
 * based on the result of the addition.
 *
 * @param cpu Pointer to the CPU instance.
 * @param mode The addressing mode used by the instruction.
 * @return The number of cycles consumed by the instruction.
 */
uint8_t handler_adc(CPU *cpu, AddressingMode mode) {
    uint16_t addr;
    uint8_t value;

    switch(mode) {
        case MODE_IMMEDIATE: addr = addr_immediate(cpu); break;
        case MODE_ZEROPAGE:  addr = addr_zeropage(cpu); break;
        case MODE_ABSOLUTE:  addr = addr_absolute(cpu); break;
        default:
            printf("Unimplemented addressing mode\n");
            return 0;
    }

    value = mem_read(cpu, addr);
    uint8_t carry_in = (cpu->flags & FLAG_C) ? 1 : 0;
    uint16_t result_16 = (uint16_t)cpu->a + (uint16_t)value + (uint16_t)carry_in;
    uint8_t final_result = (uint8_t)result_16;

    if (final_result == 0) cpu->flags |= FLAG_Z; else cpu->flags &= ~FLAG_Z;
    if (final_result & 0x80) cpu->flags |= FLAG_N; else cpu->flags &= ~FLAG_N;
    if (~(cpu->a ^ value) & (cpu->a ^ final_result) & 0x80) cpu->flags |= FLAG_V; else cpu->flags &= ~FLAG_V;
    if (result_16 & 0x100) cpu->flags |= FLAG_C; else cpu->flags &= ~FLAG_C;

    cpu->a = final_result;
    return 2;
}

/**
 * @brief Initializes the instruction table.
 *
 * This function populates the instruction table with all the implemented
 * opcodes, their handlers, addressing modes, and cycle counts.
 */
void init_instruction_table() {
    for(int i = 0; i < 256; i++)
        instruction_table[i] = (Instruction){ "???", NULL, MODE_IMPLIED, 0 };

    instruction_table[0xA9] = (Instruction){"LDA", handler_lda, MODE_IMMEDIATE, 2};
    instruction_table[0xA5] = (Instruction){"LDA", handler_lda, MODE_ZEROPAGE, 3};
    instruction_table[0xAD] = (Instruction){"LDA", handler_lda, MODE_ABSOLUTE, 4};
    instruction_table[0x69] = (Instruction){"ADC", handler_adc, MODE_IMMEDIATE, 2};
    instruction_table[0x65] = (Instruction){"ADC", handler_adc, MODE_ZEROPAGE, 3};
    instruction_table[0x6D] = (Instruction){"ADC", handler_adc, MODE_ABSOLUTE, 4};
}


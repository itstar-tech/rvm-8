/*
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
#include <stddef.h>

#define RVM_MEM_SIZE 65536

typedef struct {
    uint8_t a, x, y;
    uint16_t pc;
    uint16_t sp;
    uint8_t  flags;
    uint8_t *memory;
} CPU;

void cpu_init(CPU *cpu, uint8_t *memory, uint16_t pc_start);

void cpu_reset(CPU *cpu);

void cpu_step(CPU *cpu);

uint8_t mem_read(CPU *cpu, uint16_t addr);

void mem_write(CPU *cpu, uint16_t addr, uint8_t val);

#endif

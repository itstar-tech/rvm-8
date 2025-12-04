/*
 * rvm-8/kernel/tests/test_cpu.c
 *
 * Unit tests for the rvm-8 CPU (placeholder).
 *
 * Copyright (c) 2025 foxomax
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../cpu.h"

uint8_t memory[65536];
CPU cpu;

void setup_test() {
    memset(memory, 0, 65536);
}

void test_simple_addition() {
    printf("TEST: Simple Addition (5 + 10)...\n");
    setup_test();

    memory[0xFFFC] = 0x00;
    memory[0xFFFD] = 0x80;

    memory[0x8000] = 0xA9;
    memory[0x8001] = 0x05;

    memory[0x8002] = 0x69;
    memory[0x8003] = 0x0A;

    cpu_init(&cpu, memory);

    cpu_step(&cpu);

    assert(cpu.pc == 0x8002);
    assert(cpu.a == 5);
    assert((cpu.flags & FLAG_Z) == 0);
    assert((cpu.flags & FLAG_N) == 0);

    cpu_step(&cpu);

    assert(cpu.pc == 0x8004);
    assert(cpu.a == 15);

    printf("PASS!\n");
}

void test_overflow_carry() {
    printf("TEST: Carry Flag (255 + 1)...\n");
    setup_test();

    memory[0xFFFC] = 0x00; memory[0xFFFD] = 0x80;

    memory[0x8000] = 0xA9; memory[0x8001] = 0xFF;

    memory[0x8002] = 0x69; memory[0x8003] = 0x01;

    cpu_init(&cpu, memory);

    cpu_step(&cpu);
    cpu_step(&cpu);

    assert(cpu.a == 0);
    assert(cpu.flags & FLAG_Z);
    assert(cpu.flags & FLAG_C);

    printf("PASS!\n");
}

int main() {
    test_simple_addition();
    test_overflow_carry();

    printf("\nALL TESTS WERE PASSED.\n");
    return 0;
}

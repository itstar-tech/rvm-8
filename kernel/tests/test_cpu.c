/*
 * rvm-8/kernel/tests/test_cpu.c
 *
 * Unit tests for the rvm-8 CPU (placeholder).
 *
 * Copyright (c) 2025 foxomax
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../cpu.h"

uint8_t memory[65536];
CPU cpu;

void setup_test() { memset(memory, 0, 65536); }

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

  memory[0xFFFC] = 0x00;
  memory[0xFFFD] = 0x80;

  memory[0x8000] = 0xA9;
  memory[0x8001] = 0xFF;

  memory[0x8002] = 0x69;
  memory[0x8003] = 0x01;

  cpu_init(&cpu, memory);

  cpu_step(&cpu);
  cpu_step(&cpu);

  assert(cpu.a == 0);
  assert(cpu.flags & FLAG_Z);
  assert(cpu.flags & FLAG_C);

  printf("PASS!\n");
}

void test_lda_modes() {
  printf("TEST: LDA Addressing Modes...\n");
  setup_test();

  // 1. Immediate: LDA #$10
  memory[0x8000] = 0xA9;
  memory[0x8001] = 0x10;

  // 2. Zero Page: LDA $20
  memory[0x0020] = 0x22;
  memory[0x8002] = 0xA5;
  memory[0x8003] = 0x20;

  // 3. Absolute: LDA $3000
  memory[0x3000] = 0x33;
  memory[0x8004] = 0xAD;
  memory[0x8005] = 0x00;
  memory[0x8006] = 0x30;

  // 4. Zero Page X: LDA $20,X (X=1) => $21
  memory[0x0021] = 0x44;
  memory[0x8007] = 0xA2; // LDX #$01
  memory[0x8008] = 0x01;
  memory[0x8009] = 0xB5; // LDA $20,X
  memory[0x800A] = 0x20;

  // 5. Absolute X: LDA $4000,X (X=1) => $4001
  memory[0x4001] = 0x55;
  memory[0x800B] = 0xBD; // LDA $4000,X
  memory[0x800C] = 0x00;
  memory[0x800D] = 0x40;

  // 6. Absolute Y: LDA $5000,Y (Y=2)
  // First we need to load Y, but we don't have LDY implemented?
  // Wait, let's check cpu.h or opcodes.c. No LDY.
  // So we can manually set cpu.y for the test or skip if no instruction.
  // I'll manually set cpu.y in the C code since this is a unit test.

  // 7. Indirect X: LDA ($30,X)
  // X=1. $30+1 = $31. ptr=$31. mem[$31] = low, mem[$32] = high.
  // Let addr = $6000. mem[$31]=00, mem[$32]=60.
  // mem[$6000] = 0x77.
  memory[0x0031] = 0x00;
  memory[0x0032] = 0x60;
  memory[0x6000] = 0x77;
  // Instruction: LDA ($30,X) -> Opcode A1
  // We need to execute the instruction.

  // 8. Indirect Y: LDA ($40),Y
  // $40 -> pointer. mem[$40]=00, mem[$41]=70. Base=$7000.
  // Y=2. Addr=$7002.
  // mem[$7002] = 0x88.
  memory[0x0040] = 0x00;
  memory[0x0041] = 0x70;
  memory[0x7002] = 0x88;

  // Initial PC
  memory[0xFFFC] = 0x00;
  memory[0xFFFD] = 0x80;

  cpu_init(&cpu, memory);

  // Step 1: Immediate
  cpu_step(&cpu); // LDA #$10
  assert(cpu.a == 0x10);

  // Step 2: Zero Page
  cpu_step(&cpu); // LDA $20
  assert(cpu.a == 0x22);

  // Step 3: Absolute
  cpu_step(&cpu); // LDA $3000
  assert(cpu.a == 0x33);

  // Step 4: Zero Page X
  // First LDX #1
  cpu_step(&cpu); // LDX #1
  assert(cpu.x == 0x01);
  // Now LDA $20,X
  cpu_step(&cpu);
  assert(cpu.a == 0x44);

  // Step 5: Absolute X
  // X is still 1
  cpu_step(&cpu); // LDA $4000,X
  assert(cpu.a == 0x55);

  // Step 6: Absolute Y
  // Resetting pc logic slightly or just appending instructions?
  // I'll append.
  // Manually set Y for testing since no LDY
  cpu.y = 0x02;
  // Inject opcode in memory
  memory[cpu.pc] = 0xB9; // LDA $5000,Y
  memory[cpu.pc + 1] = 0x00;
  memory[cpu.pc + 2] = 0x50;
  memory[0x5002] = 0x66;

  cpu_step(&cpu);
  assert(cpu.a == 0x66);

  // Step 7: Indirect X
  cpu.x = 0x01;          // Ensure X=1
  memory[cpu.pc] = 0xA1; // LDA ($30,X)
  memory[cpu.pc + 1] = 0x30;

  cpu_step(&cpu);
  assert(cpu.a == 0x77);

  // Step 8: Indirect Y
  cpu.y = 0x02;          // Ensure Y=2
  memory[cpu.pc] = 0xB1; // LDA ($40),Y
  memory[cpu.pc + 1] = 0x40;

  cpu_step(&cpu);
  assert(cpu.a == 0x88);

  printf("PASS!\n");
}

int main() {
  test_simple_addition();
  test_overflow_carry();
  test_lda_modes();

  printf("\nALL TESTS WERE PASSED.\n");
  return 0;
}

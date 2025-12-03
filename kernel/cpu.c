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
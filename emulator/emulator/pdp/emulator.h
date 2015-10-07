//
//  emulator.h
//  emulator.c
//
//  Created on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#ifndef emulator_h
#define emulator_h
#include <stdint.h>
#define RAM_SIZE 4000
#define VRAM_SIZE 4000
#define ROM_SIZE 8000
#define MEMORY_SIZE 16000

struct Registers {
    uint16_t R[7];
} registers;

struct Flags {
    unsigned V: 1; // overflow
    unsigned C: 1; // carry
    unsigned Z: 1; // zero
    unsigned N: 1; // negativ
} flags;

union Memory {
    struct {
        uint16_t RAM[RAM_SIZE];
        uint16_t VRAM[VRAM_SIZE];
        uint16_t ROM[ROM_SIZE];
    };
    uint16_t memory[MEMORY_SIZE];
} memory;

int emu_run();
int emu_reset();
int emu_step();
int init_memory();

#endif /* emulator_h */

//
//  emulator.h
//
//  Created on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#ifndef emulator_h
#define emulator_h
#include <stdint.h>
#include <fcntl.h>
#define RAM_SIZE 8000
#define VRAM_SIZE 8000
#define ROM_SIZE 16000
#define MEMORY_SIZE 32000
#define FALSE 0
#define TRUE 1
#define INSTR_SIZE 16
#define LEN 256

typedef uint8_t byte;
typedef uint16_t word;

char disas[LEN];
char reg[LEN];

struct Registers {
    word R[7];
} registers;

struct Flags {
    unsigned V: 1; // overflow
    unsigned C: 1; // carry
    unsigned Z: 1; // zero
    unsigned N: 1; // negativ
} flags;

union Memory {
    struct {
        byte RAM[RAM_SIZE];
        byte VRAM[VRAM_SIZE];
        byte ROM[ROM_SIZE];
    };
    byte memory[MEMORY_SIZE];
} memory;

int stop = FALSE;

int emu_reset();
int emu_step();
int init_memory();
int emu_init();

#endif /* emulator_h */

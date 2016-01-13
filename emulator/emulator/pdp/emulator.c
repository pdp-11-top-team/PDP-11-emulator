//
//  emulator.c
//
//  Created on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#include "emulator.h"
#include "instruction_table.c"
#include <Windows.h>

FILE *file;

int init_memory() { // 00050
	int i = 0;
	int b1, b2;

	for (i = 0; i < MEMORY_SIZE; i++) {
		memory.memory[i] = 0;
	}
	for (i = 0; i < VRAM_SIZE; i++) {
		memory.VRAM[i] = i%255;
	}
	for (i = RAM_SIZE + VRAM_SIZE; i < MEMORY_SIZE; i += 2) {
		if (fscanf(file, "%x\n%x\n", &b1, &b2) >= 0) {
			memory.memory[i] = b2;
			memory.memory[i + 1] = b1;
			printf("%x %x\n", b1, b2);
		} else {
			break;
		}
	}

	return 0;
}


int init_registers() {
	int i;

	for (i = 0; i < 6; i++) {
		memory.R[i] = i + i * 16 + i * 256 + i * 4096 + 12;
	}
	memory.R[6] = RAM_SIZE + VRAM_SIZE;

	return 0;
}

int init_flags() {
    flags.C = 0;
    flags.N = 0;
    flags.V = 0;
    flags.Z = 0;
    
    return 0;
}

int emu_init() {
	file = fopen("pdp/source.txt", "r");
    init_memory();
	init_registers();
	fill_table();
    
	fclose(file);
    return 0;
}

int emu_reset() {
    init_memory();
    init_registers();
    init_flags();

    return 0;
}

byte read_mem_byte(int addr) {
	memory.R[6]++;
	return memory.memory[addr];
}

word read_mem_word(int addr) {
	memory.R[6] += 2;

	return *(word*)&memory.memory[addr];
}

int check_instr(int i, instruction in) {
	return (in.instr >= table[i].instruction_diapason.first.instr) && (in.instr <= table[i].instruction_diapason.last.instr);
}

int emu_step() {
    instruction in;
    int i;

	in.instr = read_mem_word(memory.R[6]);
	memset(disas, '0', LEN);
	memset(reg, '0', LEN);
	//log = fopen("pdp/log.txt", "a");
	//fprintf(log, "here\n");
	for (i = 0; i < COUNT; i++) {
		//fprintf(log, "%d\n", i);
        if (check_instr(i, in)) {
			handle_callback(i, in);
			//fprintf(log, "0%o %s \nR0: %d \nR1: %d \nR2: %d \nR3: %d \nR4: %d\nSP: %d \nPC: %d \n", in.instr, table[i].assembler(in), registers.R[0],registers.R[1],registers.R[2],registers.R[3],registers.R[4],registers.R[5],registers.R[6]);
			sprintf(disas, "%06o %s", in.instr, table[i].assembler(in));
			sprintf(reg, "R0: %d \r\nR1: %d \r\nR2: %d \r\nR3: %d \r\nR4: %d\r\nSP: %d \r\nPC: %d \r\n", memory.R[0], memory.R[1], memory.R[2], memory.R[3], memory.R[4], memory.R[5], memory.R[6]);
            break;
        }
    }
	//fclose(log);

    return in.instr;
}
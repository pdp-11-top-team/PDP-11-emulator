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
	int b;

	for (i = 0; i < MEMORY_SIZE; i++) {
		memory.memory[i] = 0;
	}
	for (i = 0; i < VRAM_SIZE; i++) {
		memory.VRAM[i] = 0;
	}
	for (i = RAM_SIZE + VRAM_SIZE; i < MEMORY_SIZE; i++) {
		if (fscanf(file, "%x\n", &b) >= 0) {
			memory.memory[i] = b;
		} else {
			break;
		}
	}

	return 0;
}


int init_registers() {
	int i;

	for (i = 0; i < 7; i++) {
		memory.R[i] = 0;
	}
	memory.R[7] = RAM_SIZE + VRAM_SIZE;

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
	memory.R[7]++;
	return memory.memory[addr];
}

word read_mem_word(int addr) {
	memory.R[7] += 2;

	return *(word*)&memory.memory[addr];
}

int check_instr(int i, instruction in) {
	return (in.instr >= table[i].instruction_diapason.first.instr) && (in.instr <= table[i].instruction_diapason.last.instr);
}

int emu_step() {
    instruction in;
    int i;

	in.instr = read_mem_word(memory.R[7]);
	memset(disas, '0', LEN);
	memset(reg, '0', LEN);
	//log = fopen("pdp/log.txt", "a");
	//fprintf(log, "here\n");
	for (i = 0; i < COUNT; i++) {
		//fprintf(log, "%d\n", i);
        if (check_instr(i, in)) {
			sprintf(disas, "%06o %s", in.instr, table[i].assembler(in));
			printf("%06o %s\n", in.instr, table[i].assembler(in));
			handle_callback(i, in);
			
			//fprintf(log, "0%o %s \nR0: %d \nR1: %d \nR2: %d \nR3: %d \nR4: %d\nSP: %d \nPC: %d \n", in.instr, table[i].assembler(in), registers.R[0],registers.R[1],registers.R[2],registers.R[3],registers.R[4],registers.R[5],registers.R[6]);
		
			sprintf(reg, "R0: %d \r\nR1: %d \r\nR2: %d \r\nR3: %d \r\nR4: %d\r\nR5: %d\r\nSP: %d \r\nPC: %d \r\n", memory.R[0], memory.R[1], memory.R[2], memory.R[3], memory.R[4], memory.R[5], memory.R[6], memory.R[7]);
			printf("R0: %d \r\nR1: %d \r\nR2: %d \r\nR3: %d \r\nR4: %d\r\nR5: %d\r\nSP: %d \r\nPC: %d \r\n", memory.R[0], memory.R[1], memory.R[2], memory.R[3], memory.R[4], memory.R[5], memory.R[6], memory.R[7]);

			return 1;
        }
    }
	//fclose(log);

    return 0;
}
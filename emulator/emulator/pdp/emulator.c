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

int init_memory() { 
    int i = 0;
    int b1, b2;
    
    for (i = 0; i < MEMORY_SIZE; i++) {
        memory.memory[i] = 0;
    }
	for (i = RAM_SIZE + VRAM_SIZE; i < MEMORY_SIZE; i+=2) {
        if (fscanf(file, "%x\n%x\n", &b1, &b2) >= 0) {
            memory.memory[i++] = b2;
            memory.memory[i] = b1;
        }
        else {
            break;
        }
    }
    
    return 0;
}

int init_registers() {
    int i;
    
    for (i = 0; i < 6; i++) {
        registers.R[i] = 0;
    }
    registers.R[6] = RAM_SIZE + VRAM_SIZE;
    
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
	fill_table();
	init_registers();
	file = fopen("pdp/source.txt", "r");
    init_memory();
    
	stop = FALSE;
	fclose(file);
    return 0;
}

int emu_reset() {
    stop = TRUE;
    init_memory();
    init_registers();
    init_flags();

    return 0;
}

word read_mem_word(int addr) {
    registers.R[6] += 2;
 
    return *(word*)&memory.memory[addr];
}

int check_instr(int i, instruction in) {
    return (in.instr >= table[i].instruction_diapason.first.instr)&&(in.instr <= table[i].instruction_diapason.last.instr);
}

int emu_step() {
    instruction in;
    int i;

	stop = TRUE;
	in.instr = read_mem_word(registers.R[6]);
	memset(disas, '0', LEN);
	memset(reg, '0', LEN);
	//log = fopen("pdp/log.txt", "a");
	//fprintf(log, "here\n");
	for (i = 0; i < COUNT; i++) {
		//fprintf(log, "%d\n", i);
        if (check_instr(i, in)) {
            table[i].callback(in);
			//fprintf(log, "0%o %s \nR0: %d \nR1: %d \nR2: %d \nR3: %d \nR4: %d\nSP: %d \nPC: %d \n", in.instr, table[i].assembler(in), registers.R[0],registers.R[1],registers.R[2],registers.R[3],registers.R[4],registers.R[5],registers.R[6]);
			sprintf(disas, "0%o %s", in.instr, table[i].assembler(in));
			sprintf(reg, "R0: %d \r\nR1: %d \r\nR2: %d \r\nR3: %d \r\nR4: %d\r\nSP: %d \r\nPC: %d \r\n", registers.R[0],registers.R[1],registers.R[2],registers.R[3],registers.R[4],registers.R[5],registers.R[6]);
            break;
        }
    }
	//fclose(log);

    return 0;
}
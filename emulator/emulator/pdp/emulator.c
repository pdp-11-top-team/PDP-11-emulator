//
//  emulator.c
//
//  Created on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#include "emulator.h"
#include "instruction_table.c"
#include <Windows.h>

FILE *file; // sourse.txt

int init_memory() { 
	int i = 0;
	byte curr_byte;

	for (i = 0; i < MEMORY_SIZE; i++) {
		memory.memory[i] = 0;
	}
	for (i = 0; i < VRAM_SIZE; i++) {
		memory.VRAM[i] = 0;
	}
	for (i = RAM_SIZE + VRAM_SIZE; i < MEMORY_SIZE; i++) {
		if (fscanf(file, "%x\n", &curr_byte) >= 0) {
			memory.memory[i] = curr_byte;
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
	memory.R[7] = RAM_SIZE + VRAM_SIZE; // ROM

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

word read_insrtuction(int addr) {
	memory.R[7] += 2;

	return *(word*)&memory.memory[addr];
}

int check_instr(int i, instruction in) {
	return (in.instr >= table[i].instruction_diapason.first.instr) && (in.instr <= table[i].instruction_diapason.last.instr);
}

void set_reg(char *state) {
	sprintf(reg, "R0: %d \r\nR1: %d \r\nR2: %d \r\nR3: %d \r\nR4: %d\r\nR5: %d\r\nSP: %d \r\nPC: %d \r\nState: %s", memory.R[0], memory.R[1], memory.R[2], memory.R[3], memory.R[4], memory.R[5], memory.R[6], memory.R[7], state);
}

int emu_step() {
    instruction in;
    int i;
	char *state;
	int result;

	in.instr = read_insrtuction(memory.R[7]);
	memset(disas, '0', LEN);
	memset(reg, '0', LEN);
	
	for (i = 0; i < COUNT; i++) {
        if (check_instr(i, in)) {
			sprintf(disas, "%06o %s", in.instr, table[i].assembler(in));

			result = handle_callback(i, in);
			switch(result) {
			case EX:
				state = "Execution";
				break;
			case HLT:
				state = "Completed";
				break;
			default:
				state = "Unknown error";
			}
			
			set_reg(state);

			return in.instr;
        }
    }
	sprintf(disas, "%s", "UNKNOWN COMMAND");
	set_reg("ERROR UNKNOWN OPCODE");
    return UNKNOWN_COMMAND;
}
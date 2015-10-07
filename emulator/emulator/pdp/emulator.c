//
//  emulator.c
//  emulator.c
//
//  Created on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#include "emulator.h"
#include "instruction_table.c"

int init_memory() {
    int i;
    
    for (i = 0; i < MEMORY_SIZE; i++) {
        memory.memory[i] = i + 5;
    }
    
    return 0;
}
int init_registers() {
    int i;
    
    for (i = 0; i < 7; i++) {
        registers.R[i] = i*12;
    }
    
    return 0;
}

int emu_run() {
    init_memory();
    init_registers();
    fill_table();
	FILE *file;

	file = fopen("log.txt", "w");

	if (is_bigendian()) {
		fprintf(file, "It works!");
	} else {
		instruction in;
		in.instr = 0005075;
		fprintf(file, "%s\n", clt(in));
	}

	fclose(file);
    return 0;
}

int emu_reset() {
    return 0;
}

int emu_step() {
    return 0;
}
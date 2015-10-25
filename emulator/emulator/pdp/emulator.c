//
//  emulator.c
//  emulator.c
//
//  Created by Jenny on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#include "emulator.h"
#include "instruction_table.c"
#include <Windows.h>

FILE *file, *log;

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

int init_flags() {
    flags.C = 0;
    flags.N = 0;
    flags.V = 0;
    flags.Z = 0;
    
    return 0;
}

int emu_run() {
	while (stop == FALSE) {
        emu_step();
	}
    return 0;
}

int emu_init() {
    init_memory();
    init_registers();
	init_flags();
    fill_table();
	log = fopen("pdp/log.txt", "a");
	if ((file = fopen("pdp/source.txt", "r")) == NULL) {
		fprintf(log, "Error\n");
	}
    stop = FALSE;
    
    return 0;
}

int emu_reset() {
    stop = TRUE;
	fclose(file);
	fclose(log);
	emu_init();

    return 0;
}

int emu_step() {
    instruction in;
    
	if (fscanf(file, "%ho\n", &in.instr) < 0) {
        stop = TRUE;
        return 0;
    }
    
	//in.instr = 0005075;
    
    fprintf(log, "0%o %s \nR0: %d \nR1: %d \nR2: %d \nR3: %d \nR4: %d\nSP: %d \nPC: %d \n", in.instr, clt(in), registers.R[0],
		registers.R[1],
		registers.R[2],
		registers.R[3],
		registers.R[4],
		registers.R[5],
		registers.R[6]);
    return 0;
}
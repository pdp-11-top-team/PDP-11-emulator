//
//  instruction_table.c
//  emulator.c
//
//  Created by Jenny on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "instruction_table.h"
#include "emulator.h"

struct Destination get_rd(instruction instr) {
    struct Destination dest;
    
    dest.md = instr.sa_instr.md;
    dest.rd = instr.sa_instr.rd;
    dest.dest_address = &registers.R[instr.sa_instr.rd];
    dest.dest_disas = (char *)malloc(LEN*sizeof(char));
    
    return dest;
}

byte *get_from_memory(byte address) {
    return &memory.memory[(int)address];
}

void get_md(struct Destination *dest) {
    byte addr;
    
    switch (dest->md) {
        case 0:
            sprintf(dest->dest_disas, "%%R%d", dest->rd);
            return;
        case 1:
            dest->dest_address = get_from_memory(*(dest->dest_address));
            sprintf(dest->dest_disas, "ox%x", *(dest->dest_address));
            return;
        case 2:
            addr = (*dest->dest_address)++;
            dest->dest_address = get_from_memory(addr);
            sprintf(dest->dest_disas, "ox%x", addr);
            return;
        case 3:
            addr = *get_from_memory((*dest->dest_address)++);
            dest->dest_address = get_from_memory(addr);
            sprintf(dest->dest_disas, "ox%x", addr);
            return;
        case 4:
            addr = --(*dest->dest_address);
            dest->dest_address = get_from_memory(addr);
            sprintf(dest->dest_disas, "ox%x", addr);
            return;
        case 5:
            addr = *get_from_memory(--(*dest->dest_address));
            dest->dest_address = get_from_memory(addr);
            sprintf(dest->dest_disas, "ox%x", addr);
            return;
        case 6:
            addr = (*dest->dest_address)+(*get_from_memory(registers.R[6]++));
            dest->dest_address = get_from_memory(addr);
            sprintf(dest->dest_disas, "ox%x", addr);
            return;
        case 7:
            addr = *get_from_memory((*dest->dest_address)+(*get_from_memory(registers.R[6]++)));
            dest->dest_address = get_from_memory(addr);
            sprintf(dest->dest_disas, "ox%x", addr);
            return;
        default:
            return;
    }
}

void set_flags(int n, int c, int z, int v) {
    flags.N = n;
    flags.C = c;
    flags.Z = z;
    flags.V = v;
}

char *clt(instruction instr) {
    struct Destination dest = get_rd(instr);
    get_md(&dest);
    char *disas;
    
    disas = (char *)malloc(LEN*sizeof(char));
    sprintf(disas, "CLT ");
    strcat(disas, dest.dest_disas);
    *dest.dest_address = 0;
    set_flags(0, 0, 1, 0);
    free(dest.dest_disas);
    
    return disas;
}


int fill_table(void) {
    int i = 0;
    
    if(is_bigendian()) {
        return 0;
    }
    
    table[i].instruction_diapason.first.instr = 005000;
    table[i].instruction_diapason.last.instr = 005077;
    table[i].function = clt;
    
    return 0;
}
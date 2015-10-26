//
//  instruction_table.c
//
//  Created on 06.10.15.
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
    
    return dest;
}

byte *get_from_memory(byte address) {
    return &memory.memory[address];
}

byte get_md(struct Destination *dest) {
    byte addr;
    
    switch (dest->md) {
        case 0:
            return dest->rd;
        case 1:
            addr = *(dest->dest_address);
            dest->dest_address = (word *)get_from_memory(addr);
            return addr;
        case 2:
            addr = (*dest->dest_address)++;
            dest->dest_address = (word *)get_from_memory(addr);
            return addr;
        case 3:
            addr = *get_from_memory((*dest->dest_address)++);
            dest->dest_address = (word *)get_from_memory(addr);
            return addr;
        case 4:
            addr = --(*dest->dest_address);
            dest->dest_address = (word *)get_from_memory(addr);
            return addr;
        case 5:
            addr = *get_from_memory(--(*dest->dest_address));
            dest->dest_address = (word *)get_from_memory(addr);
            return addr;
        case 6:
            addr = (*dest->dest_address)+(*get_from_memory(registers.R[6]++));
            dest->dest_address = (word *)get_from_memory(addr);
            return addr;
        case 7:
            addr = *get_from_memory((*dest->dest_address)+(*get_from_memory(registers.R[6]++)));
            dest->dest_address = (word *)get_from_memory(addr);
            return addr;
    }
    
    return WRONG_DEST;
}


char *get_md_disas(int md, byte addr) {
    char *disas = (char *)malloc(LEN*sizeof(char));
    
    switch (md) {
        case 0:
            sprintf(disas, "%%R%d", addr);
            return disas;
        default:
            sprintf(disas, "ox%x", addr);
            return disas;
    }
}


void set_flags(int n, int c, int z, int v) {
    flags.N = n;
    flags.C = c;
    flags.Z = z;
    flags.V = v;
}

void clt(instruction instr) {
    struct Destination dest = get_rd(instr);
    get_md(&dest);
    
    *dest.dest_address = 0;
    set_flags(0, 0, 1, 0);
    
    return;
}

char *clt_disas(instruction instr) {
    struct Destination dest = get_rd(instr);
    
    char *disas;
    char *dest_disas;
    
    disas = (char *)malloc(LEN*sizeof(char));
    dest_disas = get_md_disas(dest.md, get_md(&dest));
    sprintf(disas, "CLT ");
    strcat(disas, dest_disas);
    free(dest_disas);
    
    return disas;
}

int fill_table(void) {
    int i = 0;
    
    if(is_bigendian()) {
        return 0;
    }
    
   /* table[i].instruction_diapason.first.sa_instr.bw = 0;
    table[i].instruction_diapason.first.sa_instr.opcode = 0050;
    table[i].instruction_diapason.first.sa_instr.md = 0;
    table[i].instruction_diapason.first.sa_instr.rd = 0;
    ;*/
    table[i].instruction_diapason.first.instr = 005000;
   /* table[i].instruction_diapason.last.sa_instr.bw = 0;
    table[i].instruction_diapason.last.sa_instr.opcode = 0050;
    table[i].instruction_diapason.last.sa_instr.md = 7;
    table[i].instruction_diapason.last.sa_instr.rd = 7;*/
    table[i].instruction_diapason.last.instr = 005077;
    table[i].callback= clt;
    table[i].assembler = clt_disas;
    
    return 0;
}
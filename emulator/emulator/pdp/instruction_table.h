//
//  instruction_table.h
//  emulator.c
//
//  Created on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#ifndef instruction_table_h
#define instruction_table_h

#define COUNT 60
const int for_check = 1;
#define is_bigendian() ( (*(char*)&for_check) == 0 )
#define LEN 256

typedef union Instruction {
    struct Da_instr {
        unsigned rd: 3; // destination register
        unsigned md: 3; // destination addressing method
        unsigned rs: 3; // sourse register
        unsigned ms: 3; // sourse addressing method
        unsigned opcode: 3;
        unsigned bw: 1; // byte - 1, world - 0
    } da_instr; // double address instruction
    struct Sa_instr {
        unsigned rd: 3;
        unsigned md: 3;
        unsigned opcode: 9;
        unsigned bw: 1;
    } sa_instr; // single address instruction
    
    word instr;
} instruction;

typedef struct Field {
    struct Diapason {
        instruction first;
        instruction last;
    } instruction_diapason;
    char *(*function)(instruction);
    char *assembler;
} field;

field table[COUNT];

struct Destination {
    int rd;
    int md;
    uint16_t *dest_address;
    char *dest_disas;
};

struct Destination get_rd(instruction instr);
byte *get_from_memory(byte address);
void get_md(struct Destination *dest);
void set_flags(int n, int c, int z, int v);
int fill_table(void);
char *clt(instruction instr);

#endif /* instruction_table_h */

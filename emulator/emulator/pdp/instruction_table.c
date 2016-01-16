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

struct Operand get_rd(instruction instr) {
	struct Operand dest;

	if (instr.sa_instr.bw == 0) {
		dest.m = instr.sa_instr.md;
		dest.r = instr.sa_instr.rd;
		dest.address = get_word_from_memory(instr.sa_instr.rd * 2 + R0_INDEX);
	}
	else {
		dest.m = instr.da_instr.md;
		dest.r = instr.da_instr.rd;
		dest.address = get_word_from_memory(instr.da_instr.rd * 2 + R0_INDEX);
	}

	return dest;
}

struct Operand get_rs(instruction instr) {
	struct Operand source;

	source.m = instr.da_instr.ms;
	source.r = instr.da_instr.rs;
	source.address = get_word_from_memory(R0_INDEX + instr.da_instr.rs * 2);

	return source;
}

byte *get_byte_from_memory(int address) {
	return &memory.memory[address];
}

word *get_word_from_memory(int address) {
	return (word *)&memory.memory[address];
}

void put_value_b(int address, byte value) {
	byte *dest_address;

	dest_address = get_byte_from_memory(address);
	*dest_address = value;
}

void put_value_w(int address, word value) {
	word *dest_address;

	dest_address = get_word_from_memory(address);
	*dest_address = value;
}

int get_opb(struct Operand *op) {
	int addr;

	switch (op->m) {
	case 0:
		return R0_INDEX + op->r * 2;
	case 1:
		addr = *op->address;
		return addr;
	case 2:
		if (op->r == 7) {
			addr = memory.R[7];
		}
		else {
			addr = *op->address;
		}
		return addr;
	case 3:
		addr = *get_byte_from_memory(*op->address);
		return addr;
	case 4:
		addr = --(*op->address);
		return addr;
	case 5:
		addr = *get_byte_from_memory(--(*op->address));
		return addr;
	case 6:
		addr = (*op->address) + (*get_byte_from_memory(memory.R[7]));
		return addr;
	case 7:
		addr = *get_byte_from_memory((*op->address) + (*get_byte_from_memory(memory.R[7])));
		return addr;
	}

	return WRONG_DEST;
}


int get_opw(struct Operand *op) {
	int addr;

	switch (op->m) {
	case 0:
		return R0_INDEX + op->r * 2;
	case 1:
		addr = *op->address;
		return addr;
	case 2:
		if (op->r == 7) {
			addr = memory.R[7];
		}
		else {
			addr = *op->address;
		}
		//(*op->address) += 2;
		return addr;
	case 3:
		addr = *get_word_from_memory(*op->address);
		//(*op->address) += 2;
		return addr;
	case 4:
		*op->address -= 2;
		addr = *op->address;
		return addr;
	case 5:
		*op->address -= 2;
		addr = *get_word_from_memory(*op->address);
		return addr;
	case 6:
		addr = (*op->address) + (*get_word_from_memory(memory.R[7]));
		return addr;
	case 7:
		addr = *get_word_from_memory((*op->address) + (*get_word_from_memory(memory.R[7])));
		return addr;
	}

	return WRONG_DEST;
}


void handle_callback(int i, instruction instr) {
	int addr, addrs;
	int val = 0;
	struct Operand dest = get_rd(instr);
	struct Operand source;
	short disp;

	switch (table[i].type) {
	case SA:
		if (instr.sa_instr.bw == 0) {
			addr = get_opb(&dest);
			val = 1;
		}
		else {
			addr = get_opw(&dest);
			val = 2;
		}
		table[i].callback(addr, 0);
		break;
	case DA:
		source = get_rs(instr);
		if (instr.da_instr.bw == 0) {
			addr = get_opb(&dest);
			addrs = get_opb(&source);
			val = 1;
		}
		else {
			addr = get_opw(&dest);
			addrs = get_opw(&source);
			val = 2;
		}
		table[i].callback(addr, addrs);
		switch (source.m) {
		case 2: 
			if (source.r == 7) {
				memory.R[7] += 2;
			}
		case 3:
			source.address += val;
			break;
		case 6: case 7:
			memory.R[7] += 2;
			break;
		}
		break;
	case UN:
		source = get_rs(instr);
		addr = get_opw(&dest);
		addrs = get_opw(&source);
		val = 2;
		table[i].callback(addr, addrs);
		switch (source.m) {
		case 2: 
			if (source.r == 7) {
				memory.R[7] += 2;
			}
		case 3:
			source.address += val;
			break;
		case 6: case 7:
			memory.R[7] += 2;
			break;
		}
		break;
	case BR:
		disp = MAXBYTE & instr.instr;
		if (disp & SIGN) {
			disp = disp | HBYTE;
		}
		if (table[i].callback(0, 0) != 0) {
			memory.R[7] += 2 * disp;
		}
		return;
	case CTR:
		table[i].callback(addr, 0);
		return;
	default:
		return;
	}

	switch (dest.m) {
	case 2: 
		if (dest.r == 7) {
			memory.R[7] += 2;
		}
	case 3:
		dest.address += val;
		break;
	case 6: case 7:
		memory.R[7] += 2;
		break;
	}
}

char *get_opb_disas(struct Operand *op) {
	char *disas = (char *)malloc(LEN*sizeof(char));

	switch (op->m) {
	case 0:
		sprintf(disas, "%%R%d", op->r);
		return disas;
	case 2:
		if (op->r == 7) {
			sprintf(disas, "#%d", (*get_byte_from_memory(memory.R[7])));
			return disas;
		}
	case 1: case 4:
		sprintf(disas, "0x%x", *op->address);
		return disas;
	case 3: case 5:
		sprintf(disas, "0x%x", *get_byte_from_memory(*op->address));
		return disas;
	case 6:
		sprintf(disas, "0x%x", (*op->address) + (*get_byte_from_memory(memory.R[7])));
		return disas;
	default:
		sprintf(disas, "0x%x", *get_byte_from_memory((*op->address) + (*get_byte_from_memory(memory.R[7]))));
		return disas;
	}
}

char *get_opw_disas(struct Operand *op) {
	char *disas = (char *)malloc(LEN*sizeof(char));

	switch (op->m) {
	case 0:
		sprintf(disas, "%%R%d", op->r);
		return disas;
	case 2:
		if (op->r == 7) {
			sprintf(disas, "#%d", (*get_word_from_memory(memory.R[7])));
			return disas;
		}
	case 1: case 4:
		sprintf(disas, "0x%x", *op->address);
		return disas;
	case 3: case 5:
		sprintf(disas, "0x%x", *get_word_from_memory(*op->address));
		return disas;
	case 6:
		sprintf(disas, "0x%x", (*op->address) + (*get_word_from_memory(memory.R[7])));
		return disas;
	default:
		sprintf(disas, "0x%x", *get_word_from_memory((*op->address) + (*get_word_from_memory(memory.R[7]))));
		return disas;
	}
}
void set_flags(int n, int z, int v, int c) {
	flags.N = n;
	flags.C = c;
	flags.Z = z;
	flags.V = v;
}

char *sa_instr_disas(instruction instr, char *op_name) {
	struct Operand dest = get_rd(instr);

	char *disas;
	char *dest_disas;

	disas = (char *)malloc(LEN*sizeof(char));
	if (instr.sa_instr.bw == 0) {
		dest_disas = get_opb_disas(&dest);
	} else {
		dest_disas = get_opw_disas(&dest);
	}

	sprintf(disas, "%s %s", op_name, dest_disas);

	free(dest_disas);

	return disas;
}

char *da_instr_disas(instruction instr, char *op_name) {
	struct Operand dest = get_rd(instr);
	struct Operand source = get_rs(instr);

	char *disas;
	char *dest_disas, *source_disas;

	disas = (char *)malloc(LEN*sizeof(char));
	if (instr.sa_instr.bw == 0) {
		dest_disas = get_opb_disas(&dest);
		source_disas = get_opb_disas(&source);
	} else {
		dest_disas = get_opw_disas(&dest);
		source_disas = get_opw_disas(&source);
	}

	sprintf(disas, "%s %s, %s", op_name, source_disas, dest_disas);

	free(dest_disas);
	free(source_disas);

	return disas;
}

char *un_instr_disas(instruction instr, char *op_name) {
	struct Operand dest = get_rd(instr);
	struct Operand source = get_rs(instr);

	char *disas;
	char *dest_disas, *source_disas;

	disas = (char *)malloc(LEN*sizeof(char));

	dest_disas = get_opw_disas(&dest);
	source_disas = get_opw_disas(&source);

	sprintf(disas, "%s %s, %s", op_name, source_disas, dest_disas);

	free(dest_disas);
	free(source_disas);

	return disas;
}

char *br_instr_disas(instruction instr, char *op_name) {
	char *disas;

	disas = (char *)malloc(LEN*sizeof(char));
	short disp = MAXBYTE & instr.instr;
	if (disp & SIGN) {
		disp = disp | HBYTE;
	}

	sprintf(disas, "%s ox%x", op_name, memory.R[7] + 2 * disp);

	return disas;
}

int clr(int addr, int addrs) {
	put_value_w(addr, 0);
	set_flags(0, 1, 0, 0);

	return 0;
}

int clrb(int addr, int addrs) {
	put_value_b(addr, 0);
	set_flags(0, 1, 0, 0);

	return 0;
}


char *clr_disas(instruction instr) {
	return sa_instr_disas(instr, "CLR");
}

char *clrb_disas(instruction instr) {
	return sa_instr_disas(instr, "CLRB");
}

int mov(int addr, int addrs) {
	put_value_w(addr, *get_word_from_memory(addrs));
	set_flags(NPLUSWORD, ZPLUSWORD, 0, flags.C);

	return 0;
}

int movb(int addr, int addrs) {
	put_value_b(addr, *get_byte_from_memory(addrs));
	set_flags(NPLUSBYTE, ZPLUSBYTE, 0, flags.C);

	return 0;
}


char *mov_disas(instruction instr) {
	return da_instr_disas(instr, "MOV");
}

char *movb_disas(instruction instr) {
	return da_instr_disas(instr, "MOVB");
}

int inc(int addr, int addrs) {
	int v = 0;

	if (*get_word_from_memory(addr) == MAXWORD) {
		v = 1;
	}
	put_value_w(addr, *get_word_from_memory(addr) + 1);
	set_flags(NPLUSWORD, ZPLUSWORD, v, flags.C);

	return 0;
}

int incb(int addr, int addrs) {
	int v = 0;

	if (*get_byte_from_memory(addr) == MAXBYTE) {
		v = 1;
	}
	put_value_b(addr, *get_byte_from_memory(addr) + 1);
	set_flags(NPLUSBYTE, ZPLUSBYTE, v, flags.C);

	return 0;
}

char *inc_disas(instruction instr) {
	return sa_instr_disas(instr, "INC");
}

char *incb_disas(instruction instr) {
	return sa_instr_disas(instr, "INCB");
}

int cmp(int addr, int addrs) {
	int n, z;
	n = (((*get_word_from_memory(addrs) - *get_word_from_memory(addr)) & WSIGN) == 0) ? 0 : 1;
	z = ((*get_word_from_memory(addrs) - *get_word_from_memory(addr)) == 0) ? 1 : 0;
	set_flags(n, z, 0, 0); // improve

	return 0;
}

int cmpb(int addr, int addrs) {
	int n, z;
	n = (((*get_byte_from_memory(addrs) - *get_byte_from_memory(addr)) & SIGN) == 0) ? 0 : 1;
	z = ((*get_byte_from_memory(addrs) - *get_byte_from_memory(addr)) == 0) ? 1 : 0;
	set_flags(n, z, 0, 0); // improve

	return 0;
}

char *cmp_disas(instruction instr) {
	return da_instr_disas(instr, "CMP");
}

char *cmpb_disas(instruction instr) {
	return da_instr_disas(instr, "CMPB");
}

int bne(int addr, int addrs) {
	return flags.Z == 0;
}

char *bne_disas(instruction instr) {
	return br_instr_disas(instr, "BNE");
}

int jmp(int addr, int addrs) {
	memory.R[7] = addr;

	return 0;
}

char *jmp_disas(instruction instr) {
	return sa_instr_disas(instr, "JMP");
}

int add(int addr, int addrs) {
	put_value_w(addr, *get_word_from_memory(addr) + *get_word_from_memory(addrs));
	set_flags(NPLUSBYTE, ZPLUSBYTE, 0, 0); // improve!!!!!

	return 0;
}

char *add_disas(instruction instr) {
	return un_instr_disas(instr, "ADD");
}

int mul(int addr, int addrs) {
	put_value_w(addrs, (*get_word_from_memory(addr)) * (*get_word_from_memory(addrs)));
	set_flags(NPLUSBYTE, ZPLUSBYTE, 0, 0); // improve!!!!!

	return 0;
}

char *mul_disas(instruction instr) {
	return un_instr_disas(instr, "MUL");
}


void table_init() {
	table_index = 0;
}

void add_instr(word first, word last, int (*callback)(int addr, int addrs), char *(*assembler)(instruction), int type) {
	table[table_index].instruction_diapason.first.instr = first;
	table[table_index].instruction_diapason.last.instr = last;
	table[table_index].callback = callback;
	table[table_index].assembler = assembler;
	table[table_index].type = type;

	table_index++;
}

int fill_table(void) {
	if (is_bigendian()) {
		return 0;
	}

	add_instr(0105000, 0105077, clr, clr_disas, SA);
	add_instr(005000, 005077, clrb, clrb_disas, SA);
	add_instr(0110000, 0117777, mov, mov_disas, DA);
	add_instr(010000, 017777, movb, movb_disas, DA);
	add_instr(0105200, 0105277, inc, inc_disas, SA);
	add_instr(005200, 005277, incb, incb_disas, SA);
	add_instr(0120000, 0127777, cmp, cmp_disas, DA);
	add_instr(020000, 027777, cmpb, cmpb_disas, DA);
	add_instr(001000, 001377, bne, bne_disas, BR);
	add_instr(000100, 000177, jmp, jmp_disas, CTR);
	add_instr(060000, 067777, add, add_disas, UN);
	add_instr(070000, 070777, mul, mul_disas, UN);

	return 0;
}
//
//  instruction_table.h
//
//  Created on 06.10.15.
//  Copyright © 2015 com.mipt. All rights reserved.
//

#ifndef instruction_table_h
#define instruction_table_h

#define COUNT 13 // instructions in table

// endian check
const int for_check = 1;
#define is_bigendian() ( (*(char*)&for_check) == 0 )
#define WRONG_ENDIAN 1

#define LEN 256
#define WRONG_ADDR -1
#define UNKNOWN_COMMAND -2
#define MAXBYTE 255
#define MAXWORD 65535
#define HBYTE 65280
#define SIGN 128
#define WSIGN 32768
#define NBYTE(res, op) if (((op) & SIGN) == 0) { res = 0; } else { res = 1; }
#define NWORD(res, op) if (((op) & WSIGN) == 0) { res = 0; } else { res =  1; }
#define Z(res, op) if ((op) == 0) { res = 1; } else { res = 0; }
#define VBYTE(res, op) if (((op) & MAXBYTE) == (op)) { res = 0; } else { res =  1; }
#define VWORD(res, op) if (((op) & MAXWORD) == (op)) { res = 0; } else { res =  1; }
#define CBYTE(res, op) if (((op) & (MAXBYTE + 1)) == (MAXBYTE + 1)) { res = 1; } else { res = 0; }
#define CWORD(res, op) if (((op) & (MAXWORD + 1)) == (MAXWORD + 1)) { res = 1; } else { res = 0; }

enum Type { // instruction type
	SA = 1,
	DA,
	UN,
	BR,
	CTR_INT,
	CTR
};

enum State { // execution state
	EX = 0,
	HLT = 1,
};

typedef union Instruction {
	struct Da_instr {
		unsigned rd : 3; // destination register
		unsigned md : 3; // destination addressing method
		unsigned rs : 3; // sourse register
		unsigned ms : 3; // sourse addressing method
		unsigned opcode : 3;
		unsigned bw : 1; // byte - 1, world - 0
	} da_instr; // double address instruction
	struct Sa_instr {
		unsigned rd : 3;
		unsigned md : 3;
		unsigned opcode : 9;
		unsigned bw : 1; // 0000 0000 1010 0000
	} sa_instr; // single address instruction
	struct Un_instr {
		unsigned rd : 3; // destination register
		unsigned md : 3; // destination addressing method
		unsigned rs : 3; // sourse register
		unsigned ms : 3; // sourse addressing method
		unsigned opcode : 4;
	} un_instr; 

	word instr;
} instruction;

typedef struct Field {
	struct Diapason {
		instruction first;
		instruction last;
	} instruction_diapason;
	int(*callback)(int addr, int addrs); 
	char *(*assembler)(instruction);
	int type;
} field;

field table[COUNT];
int table_index;

struct Operand {
	int r; // register
	int m; // method
	word *address;
};

byte *get_byte_from_memory(int address);
word *get_word_from_memory(int address);

#endif /* instruction_table_h */

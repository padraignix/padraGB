#pragma once

#include <stdbool.h>

extern struct reg {
	struct {
		union {
			struct {
				unsigned char f;
				unsigned char a;
			};
			unsigned short af;
		};
	};
	struct {
		union {
			struct {
				unsigned char c;
				unsigned char b;
			};
			unsigned short bc;
		};
	};
	struct {
		union {
			struct {
				unsigned char e;
				unsigned char d;
			};
			unsigned short de;
		};
	};
	struct {
		union {
			struct {
				unsigned char l;
				unsigned char h;
			};
			unsigned short hl;
		};
	};
	struct {
		union {
			struct {
				unsigned char sp_lo;
				unsigned char sp_hi;
			};
		  unsigned short sp;
		};
	};
	struct {
		union {
			struct {
				unsigned char pc_lo;
				unsigned char pc_hi;
			};
		  unsigned short pc;
		};
	};
  bool ime;
  bool ei_enable;
  int ei_cycle_count;
};

extern unsigned char cpustep(char *opcode);
extern void initcpu();
extern unsigned short print_reg_af();
extern unsigned short print_reg_bc();
extern unsigned short print_reg_de();
extern unsigned short print_reg_hl();
extern unsigned short print_reg_sp();
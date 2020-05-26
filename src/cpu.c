#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <globals.h>
#include <cpu.h>

struct reg z80;

unsigned char cpustep(char *opcode)
{


}

void initcpu()
{
    z80.af=0x01B0;
	z80.bc=0x0013;
	z80.de=0x00D8;
	z80.hl=0x014D;
	z80.sp=0xFFFE;
}

unsigned short print_reg_af()
{
    return z80.af;
}
unsigned short print_reg_bc()
{
    return z80.bc;
}
unsigned short print_reg_de()
{
    return z80.de;
}
unsigned short print_reg_hl()
{
    return z80.hl;
}
unsigned short print_reg_sp()
{
    return z80.sp;
}

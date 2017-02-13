#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <globals.h>
#include <main.h>

#include <memory.h>

unsigned char loadRom(char *filename)
{
	FILE *f;
	size_t length;
	
	f = fopen(filename, "rb");
	if(!f) return 0;
	fseek(f, 0, SEEK_END);
	length = ftell(f);	
	rewind(f);
	fread(rom, length, 1, f);
	fclose(f);
	memset(name, '\0', 17);
	for(int i=0;i < 16;i++){
			name[i] = rom[0x134 + i];
	}
	printf("###################################\n");
	printf("Name - %s\n", name);

	switch (rom[0x147]){

	case 0:
	case 1:
	case 2:
	case 3:
		mbc1 = 1;
		printf("Cart Type - MBC1\n");
		break;
	case 5:
	case 6:
		mbc2 = 1;
		printf("Cart Type - MBC2\n");
		break;
	case 8:
	case 9:
		printf("Cart Type - Vanilla\n");
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		mbc3 = 1;
		printf("Cart Type - MBC3\n");
		break;
	case 22:
	case 23:
	case 24:
		mbc4 = 1;
		printf("Cart Type - MBC4\n");
		break;
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
		mbc5 = 1;
		printf("Cart Type - MBC5\n");
		break;
	default:
		printf("Cart Type - No mapper Listed\n");
		break;
	}	

	switch(rom[0x148]){
		case 0:
			printf("ROM size 32KB\n");
			break;
		case 1:
			printf("ROM size 64KB - 4 banks\n");
			break;
		case 2:
			printf("ROM size 128KB - 8 banks\n");
			break;
		case 3:
			printf("ROM size 256KB - 16 banks\n");
			break;
		case 4:
			printf("ROM size 512KB - 32 banks\n");
			break;
		case 5:
			printf("ROM size 1MB - 64 banks\n");
			break;
		case 6:
			printf("ROM size 2MB - 128 banks\n");
			break;
		case 7:
			printf("ROM size 4MB - 256 banks\n");
			break;
		case 82:
			printf("ROM size 1.1MB - 72 banks\n");
			break;
		case 83:
			printf("ROM size 1.2MB - 80 banks\n");
			break;
		case 84:
			printf("ROM size 1.5MB - 96 banks\n");
			break;
	}

	switch(rom[0x149]){
		case 0:
			printf("RAM size 0KB\n");
			break;
		case 1:
			printf("RAM size 2KB\n");
			break;
		case 2:
			printf("RAM size 8KB\n");
			break;
		case 3:
			printf("RAM size 32KB - 4 banks\n");
			break;
	}

	printf("###################################\n");
	
	return 1;
}


void reset()
{

	//reg.af=0x01B0;
	//reg.bc=0x0013;
	//reg.de=0x00D8;
	//reg.hl=0x014D;
	//reg.sp=0xFFFE;
	rom[0xFF05] = 0x00;
	rom[0xFF06] = 0x00;
	rom[0xFF07] = 0x00;
	rom[0xFF10] = 0x80;
	rom[0xFF11] = 0xBF;
	rom[0xFF12] = 0xF3;
	rom[0xFF14] = 0xBF;
	rom[0xFF16] = 0x3F;
	rom[0xFF17] = 0x00;
	rom[0xFF19] = 0xBF;
	rom[0xFF1A] = 0x7F;
	rom[0xFF1B] = 0xFF;
	rom[0xFF1C] = 0x9F;
	rom[0xFF1E] = 0xBF;
	rom[0xFF20] = 0xFF;
	rom[0xFF21] = 0x00;
	rom[0xFF22] = 0x00;
	rom[0xFF23] = 0xBF;
	rom[0xFF24] = 0x77;
	rom[0xFF25] = 0xF3;
	rom[0xFF26] = 0xF1;
	rom[0xFF40] = 0x91;
	rom[0xFF42] = 0x00;
	rom[0xFF43] = 0x00;
	rom[0xFF45] = 0x00;
	rom[0xFF47] = 0xFC;
	rom[0xFF48] = 0xFF;
	rom[0xFF49] = 0xFF;
	rom[0xFF4A] = 0x00;
	rom[0xFF4B] = 0x00;
	rom[0xFFFF] = 0x00;

	return;
}

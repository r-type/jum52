#include <stdio.h>
#include "../global.h"
#include "main.h"				// so we can access video output

//#include "hwtable.h"

// look-up table
typedef struct {
	char name[20];
	unsigned short hwaddr;
} a5200hwtable;

//#define FALSE   0
//#define TRUE    1
#define SBYTE   char

char msg[256];

//extern int count[256];
//extern BITMAP *buffer;
uint8 *memory;
static uint16 addr;

//extern uint8 *memory5200;

a5200hwtable hwtable[150] = {
// zero page locations
        "sIRQEN"  ,     0x00  ,           // shadow for IRQEN
        "RTC_HI"  ,     0x01  ,           // real time clock hi byte
        "RTC_LO"  ,     0x02  ,           // real time clock lo byte
        "CRIT_CODE_FLAG", 0x03,           // critical code flag
        "ATTRACT_TIMER", 0x04 ,           // attract mode timer/flag
        "sDLISTL" ,     0x05  ,           //Display list lo shadow
        "sDLISTH" ,     0x06  ,           //Display list hi shadow
        "sDMACTL" ,     0x07  ,           // DMA ctrl shadow
        "sCOLPM0" ,     0x08  ,          //Player/missile 0 color shadow
        "sCOLPM1" ,     0x09  ,          //Player/missile 1 color shadow
        "sCOLPM2" ,     0x0A  ,          //Player/missile 2 color shadow
        "sCOLPM3" ,     0x0B  ,          //Player/missile 3 color shadow
        "sCOLOR0" ,     0x0C  ,           //Color 0 shadow
        "sCOLOR1" ,     0x0D  ,           //Color 1 shadow
        "sCOLOR2" ,     0x0E  ,           //Color 2 shadow
        "sCOLOR3" ,     0x0F  ,            //Color 3 shadow
        "sCOLBK"  ,     0x10  ,
        "sPOT0"   ,     0x11  ,           // pot 0 shadow
        "sPOT1"   ,     0x12  ,           // pot 1 shadow
        "sPOT2"   ,     0x13  ,
        "sPOT3"   ,     0x14  ,
        "sPOT4"   ,     0x15  ,
        "sPOT5"   ,     0x16  ,
        "sPOT6"   ,     0x17  ,
        "sPOT7"   ,     0x18  ,
// page two vectors
        "ImmIRQVec",    0x200,         // immediate IRQ vector
        "ImmVBIVec",    0x202,         // immediate VBI vector
        "DfrVBIVec",    0x204,         // deferred VBI vector
        "DLIVec"   ,    0x206,         // DLI vector
        "KeyIRQVec",    0x208,         // Keyboard IRQ vector
        "KeyContVec",   0x20A,         // keypad continuation vector
        "BREAKVec" ,    0x20C,         // BREAK key IRQ vector
        "BRKIRQVec",    0x20E,         // BRK instr IRQ vector
        "SerInRdyVec",  0x210,         // serial
        "SerOutNeedVec",0x212,         // serial
        "SerOutFinVec", 0x214,         // serial
        "POKEYT1Vec",   0x216,         // POKEY timer 1 IRQ
        "POKEYT2Vec",   0x218,         // POKEY timer 2 IRQ
        "POKEYT3Vec",   0x21A,         // POKEY timer 3 IRQ
// cart stuff
       "PAL_CART",     0xBFE7,        // cart is PAL compat if == 2
       "CARTSTARTVec", 0xBFFE,
// gtia
        "HPOSP0/M0PF"  ,     0xC000,           //Horizontal position player 0
        "HPOSP1/M1PF"  ,     0xC001,           // player 1 hpos
        "HPOSP2/M2PF"  ,     0xC002,           // player 2 hpos
        "HPOSP3/M3PF"  ,     0xC003,           // player 3 hpos
        "HPOSM0/P0PF"  ,     0xC004,           // missile 0 hpos
        "HPOSM1/P1PF"  ,     0xC005,           // missile 1 hpos
        "HPOSM2/P2PF"  ,     0xC006,           // missile 2 hpos
        "HPOSM3/P3PF"  ,     0xC007,           // missile 3 hpos
        "SIZEP0/M0PL"  ,     0xC008,           // player 0 horiz size
        "SIZEP1/M1PL"  ,     0xC009,           // player 1 horiz size
        "SIZEP2/M2PL"  ,     0xC00A,           // player 2 horiz size
        "SIZEP3/M3PL"  ,     0xC00B,           // player 3 horiz size
        "SIZEM/P0PL"   ,     0xC00C,           // ???
        "GRAFP0/P1PL"  ,     0xC00D,           //
        "GRAFP1/P2PL"  ,     0xC00E,           //
        "GRAFP2/P3PL"  ,     0xC00F,           //
        "GRAFP3/TRIG0" ,     0xC010,           //
        "GRAFM/TRIG1"  ,     0xC011,           //
        "COLPM0/TRIG2" ,     0xC012,           //
        "COLPM1/TRIG3" ,     0xC013,           //
        "COLPM2/PAL"   ,     0xC014,           //
        "COLPM3"  ,     0xC015,           //
        "COLPF0"  ,		0xC016,
        "COLPF1"  ,		0xC017,
        "COLPF2"  ,		0xC018,
        "COLPF3"  ,		0xC019,
        "COLBK"   ,     0xC01A,           //
        "PRIOR"   ,     0xC01B,           //
        "VDELAY"  ,     0xC01C,           //
        "GRACTL"  ,     0xC01D,           //Graphics control
        "HITCLR"  ,		0xC01E,           // clear coll regs
        "CONSOL"  ,		0xC01F,
// antic
        "DMACTL"  ,     0xD400,           // DMA control
        "CHACTL"  ,     0xD401,           //Character control
        "DLISTL"  ,     0xD402,           //Display list lo
        "DLISTH"  ,     0xD403,           //Display list hi
        "HSCROL"  ,     0xD404,           // hscroll
        "VSCROL"  ,     0xD405,           // vscroll
        "PMBASE"  ,     0xD407,           //PM base address
        "CHBASE"  ,     0xD409,           //Character set base
        "WSYNC"   ,     0xD40A,           // wsync
        "VCOUNT"  ,     0xD40B,           // Vcount
        "PENH"    ,     0xD40C,		  // Pen horiz
        "PENV"    ,     0xD40D,           // Pen vert
        "NMIEN"   ,     0xD40E,           //NMI Enable
        "NMIRES/NMIST", 0xD40F,		  //NMI Reset / Status
// pokey regs (write/read)
	"AUDF1/POT0"   ,     0xE800,           // POKEY reg
	"AUDC1/POT1"   ,     0xE801,           // POKEY reg
	"AUDF2/POT2"   ,     0xE802,           // POKEY reg
	"AUDC2/POT3"   ,     0xE803,           // POKEY reg
	"AUDF3/POT4"   ,     0xE804,           // POKEY reg
	"AUDC3/POT5"   ,     0xE805,           // POKEY reg
	"AUDF4/POT6"   ,     0xE806,           // POKEY reg
	"AUDC4/POT7"   ,     0xE807,           // POKEY reg
	"AUDCTL/ALLPOT",0xE808,           // audio control reg
	"STIMER/KBCODE",0xE809,
	"SKREST/RANDOM",0xE80A,
	"POTGO"   ,     0xE80B,
	"SEROUT/SERIN", 0xE80D,
	"IRQEN/IRQST",  0xE80E,
	"SKCTL/SKSTAT", 0xE80F,

// reset vector
	"RESETVec",     0xFFFC
};

unsigned int disassemble(uint16 addr1, uint16 addr2);
void show_opcode(char *mystring, uint8 instr);
void show_operand(char *mystring, uint8 instr);


char *check_for_hwaddr(uint16 thisaddr) {
     int i;

     // scan hw table for this addr
     for(i=0; i<103; i++) {
         if(thisaddr == hwtable[i].hwaddr) return hwtable[i].name;
     }

     return (char *)NULL;
}

// add a hardware addr tag to the input string if neccessary
void add_hw_tag(char *s, uint16 addr) {
	int i, len;
	char *hwname;
    hwname = check_for_hwaddr(addr);
    if(hwname) {
		len = strlen(s);
		for(i=0; i<12-len; i++)
			strcat(s, " ");
		strcat(s, hwname);
	}
}

void hexview(uint16 viewaddr) {
     int i, count;
     count = 0;
     memory = memory5200;
     clrEmuScreen(0x00);
     for(i=viewaddr; i < viewaddr + 160; i++) {
         if((count % 8) == 0) {
			 sprintf(msg, "%4X: ", i);
			 printXY(msg, 0, (count / 8) * 8, 14);
		 }
		 sprintf(msg, "%02X", memory[i]);
         printXY(msg, 48 + 32 * (count % 8), (count / 8) * 8, 14);
         count++;
     }
	 printXY("Press any key to continue...", 0, 230, 15);
	 BlitBuffer(0, 240);
}

//List interrupt vectors
void listvectors(void) {
/*                      Page Two Vectors
$200 Immediate IRQ vector
$202 Immediate VBI vector
$204 Deferred VBI vector
$206 DLI vector
$208 Keyboard IRQ vector
$20A Keypad routine continuation vector
$20C BREAK key IRQ vector
$20E BRK instruction IRQ vector
$210 Serial Input Data Ready IRQ vector
$212 Serial Output Data Needed IRQ vector
$214 Serial Output Finished IRQ vector
$216 POKEY Timer 1 IRQ vector
$218 POKEY Timer 2 IRQ vector
$21A POKEY Timer 4 IRQ vector
*/
	memory = memory5200;
	clrEmuScreen(0x00);
	printXY("Page Two IRQ Vectors:", 8, 0, 15);
	sprintf(msg, "$200 Imm IRQ: %02X%02X", memory[0x201], memory[0x200]);
	printXY(msg, 8, 16, 15);
	sprintf(msg, "$202 Imm VBI: %02X%02X", memory[0x203], memory[0x202]);
	printXY(msg, 8, 24, 15);
	sprintf(msg, "$204 Def VBI: %02X%02X", memory[0x205], memory[0x204]);
	printXY(msg, 8, 32, 15);
	sprintf(msg, "$206 DLI Vec: %02X%02X", memory[0x207], memory[0x206]);
	printXY(msg, 8, 40, 15);
	sprintf(msg, "$208 Key IRQ:  %02X%02X", memory[0x209], memory[0x208]);
	printXY(msg, 8, 48, 15);
	sprintf(msg, "$20A Key cont: %02X%02X", memory[0x20B], memory[0x20A]);
	printXY(msg, 8, 56, 15);
	sprintf(msg, "$20C BREAK Key IRQ: %02X%02X", memory[0x20D], memory[0x20C]);
	printXY(msg, 8, 64, 15);
	sprintf(msg, "$20E BRK instr IRQ: %02X%02X", memory[0x20F], memory[0x20E]);
	printXY(msg, 8, 72, 15);
	//textprintf(buffer, font, 8, 80, 15, "$210 Ser Inp Data Rdy: %02X%02X", memory[0x211], memory[0x210]);
	//textprintf(buffer, font, 8, 88, 15, "$212 Ser Out Data Req: %02X%02X", memory[0x213], memory[0x212]);
	//textprintf(buffer, font, 8, 96, 15, "$214 Ser Out Complete: %02X%02X", memory[0x215], memory[0x214]);
	//textprintf(buffer, font, 8, 104, 15, "$216 Pokey T1: %02X%02X", memory[0x217], memory[0x216]);
	//textprintf(buffer, font, 8, 112, 15, "$218 Pokey T2: %02X%02X", memory[0x219], memory[0x218]);
	//textprintf(buffer, font, 8, 120, 15, "$21A Pokey T3: %02X%02X", memory[0x21B], memory[0x21A]);
	printXY("Press any key to continue...", 0, 230, 15);
	BlitBuffer(0, 240);
}

void printhelp(int x, int y) {
	clrEmuScreen(0x00);
	printXY("AVAILABLE COMMANDS:", 0, 0, 31);
	printXY("<Enter>   Disassemble at current address.", 0, 8, 29);
	printXY("s         Step thru current instruction.", 0, 16, 29);
	printXY("S         Step thru current scanline.", 0, 24, 29);
	printXY("f         run till next frame / VBI", 0, 32, 29);
	printXY("r         Run / Resume", 0, 40, 29);
	printXY("D <addr>  disassemble address <addr>", 0, 48, 29);
	printXY("v         View hexdump at current address", 0, 56, 29);
	printXY("V <addr>  View hexdump at address <addr>", 0, 64, 29);
	printXY("l         View display list data", 0, 72, 29);
	printXY("c         View character set", 0, 80, 29);
	printXY("p         View player/missile data", 0, 88, 29);
	printXY("1         Show collision registers", 0, 96, 29);
	printXY("i         List interrupt vectors", 0, 104, 29);
	printXY("T <addr>  run To <addr>", 0, 112, 29);
	printXY("K         trigger Keyboard interrupt", 0, 120, 29);
	printXY("B         trigger Break interrupt", 0, 128, 29);
	printXY("0         Set TRIG0 to 0", 0, 136, 29);
	printXY("Q or quit Quit / Exit", 0, 144, 29);
	//printXY("Press any key to continue...", 0, 230, 29);
	BlitBuffer(x, y);
}


// disassemble from address 1 to address 2
unsigned int disassemble(uint16 addr1, uint16 addr2)
{
	uint8 instr;
	int count, line;
    char string1[128];
    char string2[128];

	addr = addr1;

	count = (addr2 == 0) ? 24 : 0;
    line = 0;
    memory = memory5200;

    clrEmuScreen(0x00);
	while ((addr < addr2 || count > 0) && (addr < 0xFFFC)) {
		sprintf(msg, "%x: ", addr);
		printXY(msg, 0, line * 8, 11);

		instr = memory[addr];
		addr++;

        sprintf(string2, "  ");
		show_opcode(string1, instr);
		show_operand(string2, instr);
		sprintf(msg, "%s    %s", string1, string2);
		printXY(msg, 48, line * 8, 12);
		line++;
		if (count > 0)
			count--;
	}
	BlitBuffer(0, 240);

	return addr;
}

void show_opcode(char *mystring, uint8 instr)
{
	switch (instr) {
	case 0x6d:
	case 0x65:
	case 0x69:
	case 0x79:
	case 0x7d:
	case 0x61:
	case 0x71:
	case 0x75:
		sprintf(mystring, "ADC");
		break;
	case 0x2d:
	case 0x25:
	case 0x29:
	case 0x39:
	case 0x3d:
	case 0x21:
	case 0x31:
	case 0x35:
		sprintf(mystring, "AND");
		break;
	case 0x0e:
	case 0x06:
	case 0x1e:
	case 0x16:
		sprintf(mystring, "ASL");
		break;
	case 0x0a:
		sprintf(mystring, "ASL A");
		break;
	case 0x90:
		sprintf(mystring, "BCC");
		break;
	case 0xb0:
		sprintf(mystring, "BCS");
		break;
	case 0xf0:
		sprintf(mystring, "BEQ");
		break;
	case 0x2c:
	case 0x24:
		sprintf(mystring, "BIT");
		break;
	case 0x30:
		sprintf(mystring, "BMI");
		break;
	case 0xd0:
		sprintf(mystring, "BNE");
		break;
	case 0x10:
		sprintf(mystring, "BPL");
		break;
	case 0x00:
		sprintf(mystring, "BRK");
		break;
	case 0x50:
		sprintf(mystring, "BVC");
		break;
	case 0x70:
		sprintf(mystring, "BVS");
		break;
	case 0x18:
		sprintf(mystring, "CLC");
		break;
	case 0xd8:
		sprintf(mystring, "CLD");
		break;
	case 0x58:
		sprintf(mystring, "CLI");
		break;
	case 0xb8:
		sprintf(mystring, "CLV");
		break;
	case 0xcd:
	case 0xc5:
	case 0xc9:
	case 0xdd:
	case 0xd9:
	case 0xc1:
	case 0xd1:
	case 0xd5:
		sprintf(mystring, "CMP");
		break;
	case 0xec:
	case 0xe4:
	case 0xe0:
		sprintf(mystring, "CPX");
		break;
	case 0xcc:
	case 0xc4:
	case 0xc0:
		sprintf(mystring, "CPY");
		break;
	case 0xce:
	case 0xc6:
	case 0xde:
	case 0xd6:
		sprintf(mystring, "DEC");
		break;
	case 0xca:
		sprintf(mystring, "DEX");
		break;
	case 0x88:
		sprintf(mystring, "DEY");
		break;
	case 0x4d:
	case 0x45:
	case 0x49:
	case 0x5d:
	case 0x59:
	case 0x41:
	case 0x51:
	case 0x55:
		sprintf(mystring, "EOR");
		break;
	case 0xff:					/* [unofficial] */
		sprintf(mystring, "ESC");
		break;
	case 0xee:
	case 0xe6:
	case 0xfe:
	case 0xf6:
		sprintf(mystring, "INC");
		break;
	case 0xe8:
		sprintf(mystring, "INX");
		break;
	case 0xc8:
		sprintf(mystring, "INY");
		break;
	case 0x4c:
	case 0x6c:
		sprintf(mystring, "JMP");
		break;
	case 0x20:
		sprintf(mystring, "JSR");
		break;
	case 0xa3:
	case 0xa7:
	case 0xaf:					/* [unofficial] */
	case 0xb3:
	case 0xb7:
	case 0xbf:
		sprintf(mystring, "LAX");
		break;
	case 0xad:
	case 0xa5:
	case 0xa9:
	case 0xbd:
	case 0xb9:
	case 0xa1:
	case 0xb1:
	case 0xb5:
		sprintf(mystring, "LDA");
		break;
	case 0xae:
	case 0xa6:
	case 0xa2:
	case 0xbe:
	case 0xb6:
		sprintf(mystring, "LDX");
		break;
	case 0xac:
	case 0xa4:
	case 0xa0:
	case 0xbc:
	case 0xb4:
		sprintf(mystring, "LDY");
		break;
	case 0x4e:
	case 0x46:
	case 0x5e:
	case 0x56:
		sprintf(mystring, "LSR");
		break;
	case 0x4a:
		sprintf(mystring, "LSR A");
		break;
	case 0xea:
		sprintf(mystring, "NOP");
		break;
	case 0x0d:
	case 0x05:
	case 0x09:
	case 0x1d:
	case 0x19:
	case 0x01:
	case 0x11:
	case 0x15:
		sprintf(mystring, "ORA");
		break;
	case 0x48:
		sprintf(mystring, "PHA");
		break;
	case 0x08:
		sprintf(mystring, "PHP");
		break;
	case 0x68:
		sprintf(mystring, "PLA");
		break;
	case 0x28:
		sprintf(mystring, "PLP");
		break;
	case 0x2e:
	case 0x26:
	case 0x3e:
	case 0x36:
		sprintf(mystring, "ROL");
		break;
	case 0x2a:
		sprintf(mystring, "ROL A");
		break;
	case 0x6e:
	case 0x66:
	case 0x7e:
	case 0x76:
		sprintf(mystring, "ROR");
		break;
	case 0x6a:
		sprintf(mystring, "ROR A");
		break;
	case 0x40:
		sprintf(mystring, "RTI");
		break;
	case 0x60:
		sprintf(mystring, "RTS");
		break;
	case 0xed:
	case 0xe5:
	case 0xe9:
	case 0xfd:
	case 0xf9:
	case 0xe1:
	case 0xf1:
	case 0xf5:
		sprintf(mystring, "SBC");
		break;
	case 0x38:
		sprintf(mystring, "SEC");
		break;
	case 0xf8:
		sprintf(mystring, "SED");
		break;
	case 0x78:
		sprintf(mystring, "SEI");
		break;
	case 0x8d:
	case 0x85:
	case 0x9d:
	case 0x99:
	case 0x81:
	case 0x91:
	case 0x95:
		sprintf(mystring, "STA");
		break;
	case 0x8e:
	case 0x86:
	case 0x96:
		sprintf(mystring, "STX");
		break;
	case 0x8c:
	case 0x84:
	case 0x94:
		sprintf(mystring, "STY");
		break;
	case 0xaa:
		sprintf(mystring, "TAX");
		break;
	case 0xa8:
		sprintf(mystring, "TAY");
		break;
	case 0xba:
		sprintf(mystring, "TSX");
		break;
	case 0x8a:
		sprintf(mystring, "TXA");
		break;
	case 0x9a:
		sprintf(mystring, "TXS");
		break;
	case 0x98:
		sprintf(mystring, "TYA");
		break;
	default:
		sprintf(mystring, "*** ILLEGAL INSTRUCTION (%x) ***", instr);
		break;
	}
}

void show_operand(char *mystring, uint8 instr)
{
	uint8 byte;
	uint16 word;

	switch (instr) {
/*
   =========================
   Absolute Addressing Modes
   =========================
 */
	case 0x6d:					/* ADC */
	case 0x2d:					/* AND */
	case 0x0e:					/* ASL */
	case 0x2c:					/* BIT */
	case 0xcd:					/* CMP */
	case 0xec:					/* CPX */
	case 0xcc:					/* CPY */
	case 0xce:					/* DEC */
	case 0x4d:					/* EOR */
	case 0xee:					/* INC */
	case 0x4c:					/* JMP */
	case 0x20:					/* JSR */
	case 0xaf:					/* LAX [unofficial] */
	case 0xad:					/* LDA */
	case 0xae:					/* LDX */
	case 0xac:					/* LDY */
	case 0x4e:					/* LSR */
	case 0x0d:					/* ORA */
	case 0x2e:					/* ROL */
	case 0x6e:					/* ROR */
	case 0xed:					/* SBC */
	case 0x8d:					/* STA */
	case 0x8e:					/* STX */
	case 0x8c:					/* STY */
		word = (memory[addr + 1] << 8) | memory[addr];
		sprintf(mystring, "$%x", word);
		add_hw_tag(mystring, word);
		addr += 2;
		break;
/*
   ======================
   0-Page Addressing Mode
   ======================
 */
	case 0x65:					/* ADC */
	case 0x25:					/* AND */
	case 0x06:					/* ASL */
	case 0x24:					/* BIT */
	case 0xc5:					/* CMP */
	case 0xe4:					/* CPX */
	case 0xc4:					/* CPY */
	case 0xc6:					/* DEC */
	case 0x45:					/* EOR */
	case 0xe6:					/* INC */
	case 0xa7:					/* LAX [unofficial] */
	case 0xa5:					/* LDA */
	case 0xa6:					/* LDX */
	case 0xa4:					/* LDY */
	case 0x46:					/* LSR */
	case 0x05:					/* ORA */
	case 0x26:					/* ROL */
	case 0x66:					/* ROR */
	case 0xe5:					/* SBC */
	case 0x85:					/* STA */
	case 0x86:					/* STX */
	case 0x84:					/* STY */
		byte = memory[addr];
		sprintf(mystring, "$%x", byte);
		add_hw_tag(mystring, (uint16)byte);
		addr++;
		break;
/*
   ========================
   Relative Addressing Mode
   ========================
 */
	case 0x90:					/* BCC */
	case 0xb0:					/* BCS */
	case 0xf0:					/* BEQ */
	case 0x30:					/* BMI */
	case 0xd0:					/* BNE */
	case 0x10:					/* BPL */
	case 0x50:					/* BVC */
	case 0x70:					/* BVS */
		byte = memory[addr];
		addr++;
		sprintf(mystring, "$%x", addr + (SBYTE) byte);
		break;
/*
   =========================
   Immediate Addressing Mode
   =========================
 */
	case 0x69:					/* ADC */
	case 0x29:					/* AND */
	case 0xc9:					/* CMP */
	case 0xe0:					/* CPX */
	case 0xc0:					/* CPY */
	case 0x49:					/* EOR */
	case 0xa9:					/* LDA */
	case 0xa2:					/* LDX */
	case 0xa0:					/* LDY */
	case 0x09:					/* ORA */
	case 0xe9:					/* SBC */
	case 0xff:					/* ESC */
		byte = memory[addr];
		addr++;
		sprintf(mystring, "#$%x", byte);
		break;
/*
   =====================
   ABS,X Addressing Mode
   =====================
 */
	case 0x7d:					/* ADC */
	case 0x3d:					/* AND */
	case 0x1e:					/* ASL */
	case 0xdd:					/* CMP */
	case 0xde:					/* DEC */
	case 0x5d:					/* EOR */
	case 0xfe:					/* INC */
	case 0xbd:					/* LDA */
	case 0xbc:					/* LDY */
	case 0x5e:					/* LSR */
	case 0x1d:					/* ORA */
	case 0x3e:					/* ROL */
	case 0x7e:					/* ROR */
	case 0xfd:					/* SBC */
	case 0x9d:					/* STA */
		word = (memory[addr + 1] << 8) | memory[addr];
		sprintf(mystring, "$%x, X", word);
		add_hw_tag(mystring, word);
		addr += 2;
		break;
/*
   =====================
   ABS,Y Addressing Mode
   =====================
 */
	case 0x79:					/* ADC */
	case 0x39:					/* AND */
	case 0xd9:					/* CMP */
	case 0x59:					/* EOR */
	case 0xbf:					/* LAX [unofficial] */
	case 0xb9:					/* LDA */
	case 0xbe:					/* LDX */
	case 0x19:					/* ORA */
	case 0xf9:					/* SBC */
	case 0x99:					/* STA */
		word = (memory[addr + 1] << 8) | memory[addr];
		sprintf(mystring, "$%x, Y", word);
		add_hw_tag(mystring, word);
		addr += 2;
		break;
/*
   =======================
   (IND,X) Addressing Mode
   =======================
 */
	case 0x61:					/* ADC */
	case 0x21:					/* AND */
	case 0xc1:					/* CMP */
	case 0x41:					/* EOR */
	case 0xa3:					/* LAX [unofficial] */
	case 0xa1:					/* LDA */
	case 0x01:					/* ORA */
	case 0xe1:					/* SBC */
	case 0x81:					/* STA */
		byte = memory[addr];
		addr++;
		sprintf(mystring, "($%x,X)", byte);
		break;
/*
   =======================
   (IND),Y Addressing Mode
   =======================
 */
	case 0x71:					/* ADC */
	case 0x31:					/* AND */
	case 0xd1:					/* CMP */
	case 0x51:					/* EOR */
	case 0xb3:					/* LAX [unofficial] */
	case 0xb1:					/* LDA */
	case 0x11:					/* ORA */
	case 0xf1:					/* SBC */
	case 0x91:					/* STA */
		byte = memory[addr];
		addr++;
		sprintf(mystring, "($%x),Y", byte);
		break;
/*
   ========================
   0-Page,X Addressing Mode
   ========================
 */
	case 0x75:					/* ADC */
	case 0x35:					/* AND */
	case 0x16:					/* ASL */
	case 0xd5:					/* CMP */
	case 0xd6:					/* DEC */
	case 0x55:					/* EOR */
	case 0xf6:					/* INC */
	case 0xb5:					/* LDA */
	case 0xb4:					/* LDY */
	case 0x56:					/* LSR */
	case 0x15:					/* ORA */
	case 0x36:					/* ROL */
	case 0x76:					/* ROR */
	case 0xf5:					/* SBC */
	case 0x95:					/* STA */
	case 0x94:					/* STY */
		byte = memory[addr];
		addr++;
		sprintf(mystring, "$%x, X", byte);
		add_hw_tag(mystring, (uint16)byte);
		break;
/*
   ========================
   0-Page,Y Addressing Mode
   ========================
 */
	case 0xb7:					/* LAX [unofficial] */
	case 0xb6:					/* LDX */
	case 0x96:					/* STX */
		byte = memory[addr];
		addr++;
		sprintf(mystring, "$%x, Y", byte);
		add_hw_tag(mystring, (uint16)byte);
		break;
/*
   ========================
   Indirect Addressing Mode
   ========================
 */
	case 0x6c:					/* printf ("JMP INDIRECT at %x\n",instr_addr); */
		word = (memory[addr + 1] << 8) | memory[addr];
		sprintf(mystring, "($%x)", word);
		add_hw_tag(mystring, word);
		addr += 2;
		break;
	}
}

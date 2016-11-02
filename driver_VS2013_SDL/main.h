#ifndef _MAIN_H_
#define _MAIN_H_

extern char msg[];

// stuff needed by the driver's files
// (external to the main Jum52 emulation core)
void clrEmuScreen(unsigned char colour);
void printXY(char *s, int x, int y, unsigned int colour);
void BlitBuffer(int destX, int destY);

// stuff for debugger
extern int nmi_busy, irq_busy, irq_pending;

extern int vcount, tickstoHSYNC, tickstilldraw;
extern uint16 dladdr, scanaddr;
extern int scanline, finished, count, dli, next_dli;
extern int current_mode, mode_y;		// running current mode & y-offset in that mode
extern int next_mode_line;
extern int bytes_per_line;
extern int framesdrawn;
extern int stolencycles;               // no of cycles stolen in this horiz line

void printhelp(int x, int y);
unsigned int disassemble(uint16 addr1, uint16 addr2);
void hexview(uint16 viewaddr);
void display_charset(void);
void listvectors(void);
int monitor(void);

struct ROMdata {
	char name[40]; // These values should be more than enough
	char filename[256];
};


char* menu_main();
int menu_update_input();
void ProcessROMlist();

#endif /* _MAIN_H_ */

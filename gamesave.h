// header file for JUM52/gamesave.c

// state struct
typedef struct {
    unsigned char A, X, Y, F, S, pad1;
    unsigned short PC;
    unsigned char IrqEn, IrqSt;
} STATE_5200;

// 6502 regs
extern unsigned char a_reg,x_reg,y_reg,flag_reg,s_reg;
extern unsigned short pc_reg;

// The RAM
extern unsigned char *memory5200;           // pointer to 6502 memory

// irq
extern unsigned char irqen, irqst;

// forget about nmien & nmist for now (just a headache)

extern char errormsg[256];

extern STATE_5200 state;


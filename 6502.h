/* for DEBUG / MONITOR purposes */
#define MONITOR

/* Macros for convenience */
#define A a_reg
#define X x_reg
#define Y y_reg
#define P flag_reg
#define S s_reg
#define PC pc_reg

#define N_FLAG 0x80
#define V_FLAG 0x40
#define G_FLAG 0x20
#define B_FLAG 0x10
#define D_FLAG 0x08
#define I_FLAG 0x04
#define Z_FLAG 0x02
#define C_FLAG 0x01

extern char space[];

// req for MSVC
void irq6502(void);

/* must be called first to initialise all 6502 engines arrays */
void init6502(void);

/* sets all of the 6502 registers. The program counter is set from
 * locations $FFFC and $FFFD masked with the above addrmask
 */
void reset6502(void);

/* run the 6502 engine for specified number of clock cycles */
void exec6502(int n);

void exec6502fast(int n);
void exec6502debug(int n);

void nmi6502();
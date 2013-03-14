// Load / Save game state

#include <stdio.h>
#include "gamesave.h"


STATE_5200 state;

// Load 5200 emu state
int LoadState(char *filename)
{
#ifdef _EE
    return PS2_LoadState( filename );
#else
	int i, flen;
	unsigned char *pc;
    FILE *pfile;
	
    pfile = fopen( filename, "rb" );
    if (pfile == NULL)
    {
    	sprintf(errormsg, "Unable to find state file to load.");
    	return -1;
    }
    		
	// load state from disk
    flen = sizeof(STATE_5200);
    pc = (unsigned char *)&state;
	for(i=0; i<flen; i++) *pc++ = fgetc(pfile);
	// copy RAM in
	pc = memory5200;
	for(i=0; i<65536; i++) *pc++ = fgetc(pfile);
	fclose(pfile);

	// restore state
	a_reg = state.A;
	x_reg = state.X;
	y_reg = state.Y;
	flag_reg = state.F;
	s_reg = state.S;
	pc_reg = state.PC;
	irqen = state.IrqEn;
	irqst = state.IrqSt;
	
	return 0;
#endif
}

// Save 5200 emu state
int SaveState(char *filename)
{
#ifdef _EE
    return PS2_SaveState( filename );
#else
	int i, flen;
	unsigned char *pc;
    FILE *pfile;
	
    pfile = fopen( filename, "wb" );
    if (pfile == NULL)
    {
    	sprintf(errormsg, "Unable to open state file for saving.");
    	return -1;
    }

	// set state
	state.A = a_reg;
	state.X = x_reg;
	state.Y = y_reg;
	state.F = flag_reg;
	state.S = s_reg;
	state.PC = pc_reg;
	state.IrqEn = irqen;
	state.IrqSt = irqst;

	// save state to disk
    flen = sizeof(STATE_5200);
    pc = (unsigned char *)&state;
	for(i=0; i<flen; i++) fputc(pc[i], pfile);
	// save RAM
	pc = memory5200;
	for(i=0; i<65536; i++) fputc(pc[i], pfile);
	fclose(pfile);

	
	return 0;
#endif
}



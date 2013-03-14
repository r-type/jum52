// MSDOS + Allegro "driver" for Jum52
//
// James Higgs 2000
//
// Notes: Compiles with DJGPP and Allegro 3.12
//        (may have to be modified slightly for later versions of Allegro)


#include <stdio.h>
//#include <stdlib.h>
#include <allegro.h>

#include "../global.h"
#include "../osdepend.h"

#include "../5200.h"
#include "../pokey.h"

BITMAP *gfxdest;

#define AUDBUFSIZE		2048
AUDIOSTREAM *audbuf;
int sound_on;
int sound_vol;
// Function proto's
int start5200sound(void);
int init5200sound(void);

// FPS counter stuff
// ALLEGRO DEPENDANT
int fps;
int fps_on;
volatile int frame;
volatile int fcounter;
int framespeed;				// 0 = no throttle, 1 = 60 Hz (50Hz), 2 = 30 Hz (25Hz)
extern int framesdrawn;
int prev_retrace_count = 0;

// timer interrupt function to count fps
void mytimerhandler()
{
	frame++;
	fcounter++;
}
END_OF_FUNCTION(mytimerhandler);


// *********************** SOUND STUFF *****************************
// 5200 sound emulation
// Jum Hig 1999
// uses Allegro audio streaming with POKEY sound emulation
// (see pokey.c)
//
// initialise sound emulation
// (returns 0 on success)
int init5200sound(void) {
	int i;
	// init POKEY (sample rate = 21280, 1 pokey chip)
	printf("Initialising POKEY emulation...\n");
	Pokey_sound_init(FREQ_17_APPROX, 21280, 1);

	// start audio streamer
	printf("Initialising audio stream...\n");
	i = start5200sound();
    return i;
}

// start playing emulated audio
// (returns 0 on success)
int start5200sound(void) {
	// create a buffer and start it playing
	// AUDIOSTREAM *play_audio_stream(int len, bits, stereo, freq, vol, pan);
	// len = 3 * 21280 / 60 = 1064
	// must be 8 bits, 'cos that's what pokeysound outputs
    // NB: 3rd arg is "STEREO" flag ???
	audbuf = play_audio_stream(AUDBUFSIZE, 8, 0, 21280, sound_vol, 128);

	// check if stream open failed
	if(audbuf == NULL) {
		printf("ERROR: audio stream open failed!\n");
		return 1;
	}
    else {
        printf("Audio stream created and started.\n");
        return 0;
	}
}

// stop playing emulated audio
void stop5200sound(void) {
	if(audbuf != NULL) {
		stop_audio_stream(audbuf);
		printf("Audio stream stopped and destroyed.\n");
	}
}

// free up audio resources
void kill5200sound(void) {
	if(audbuf != NULL) {
		stop_audio_stream(audbuf);
		printf("Audio stream stopped and destroyed.\n");
	}
}

// check and update stream buffer
// (should be called every 1/60th second, ie: every frame)
void stream_pokey(void) {
	int i;
	unsigned char *pbuf, *pbufgfx;
	// check if we need to update buffer
	pbuf = get_audio_stream_buffer(audbuf);
    pbufgfx = pbuf;
	if(pbuf != NULL) {
    	//fprintf(stderr, "updating snd buffer\n");
		// update buffer with new data
		Pokey_process(pbuf, AUDBUFSIZE);
		// tell streamer that new data is ready
		free_audio_stream_buffer(audbuf);
	}
}		


// *********************** DRIVER INTERFACE ***************************
// Prepare host system for setting the palette
void HostPrepareForPaletteSet(void) {
}

// Set palette entries - assumes 0-255 for each entry
void HostSetPaletteEntry(uint8 entry, uint8 red, uint8 green, uint8 blue) {
	// use Allegro set_color
	int i;
	RGB col;

	col.r = red >> 2;
	col.g = green >> 2;
	col.b = blue >> 2;
	set_color(entry, &col);
	
    // DEBUG
    for(i=0; i<256; i++) {
		vline(screen, i, 256, 264, i);
    }
	
    //get_palette(pal5200);
}

// Required for all the previous changes to take effect
void HostRefreshPalette(void) {
}

// Actual video copy
void HostBlitVideo(void) {
	//printf("***** BLIT *****\n");
	// Calculate our frames per second by using the
	// 60Hz extern volatile retrace_count.

    // speed controlling code goes here!
    // wait for frame counter
    while(frame < framespeed) vsync();
    frame = 0;
    // calculate fps
    if(videomode == NTSC) {
        if(fcounter > 59) {
            fps = framesdrawn;
            framesdrawn = 0;
            fcounter = 0;
        }
    }
    else {
        if(fcounter > 49) {
            fps = framesdrawn;
            framesdrawn = 0;
            fcounter = 0;
        }
    }

	if(fps_on) textprintf(gfxdest, font, 8, 0, 15, "fps: %d\n", fps);
	blit(gfxdest, screen, 0, 0, 0, 0, 384, 240);
}

// Check key status
// Also, allow OS to grab  CPU to process it's own events here if neccesary
void HostDoEvents(void) {
	// read keyboard and fill cont1 & cont2 structures
	char pckey;

	// trigger and side buttons off unless pressed below
	cont1.trig = 0;
	cont2.trig = 0;
    cont1.side_button = 0;
    cont2.side_button = 0;

	// joystick is off unless pressed below
	cont1.left = cont1.right = cont1.up = cont1.down = 0;
	cont2.left = cont2.right = cont2.up = cont2.down = 0;

	// key off is handled by 52emu.c

	//printf("Host_DoEvents()\n");
    //if(joy[0].button[0].b || (mouse_b & 1) || key[KEY_LCONTROL]) trig[0] = 0;


	if(key[KEY_LEFT]) cont1.left = 1;
	else if(key[KEY_RIGHT]) cont1.right = 1;

	if(key[KEY_UP]) cont1.up = 1;
	else if(key[KEY_DOWN]) cont1.down = 1;

	if(key[KEY_SPACE]) cont1.trig = 1;
    
    if(key[KEY_LCONTROL]) cont1.side_button = 1;

	// Load / Save state
    // Note: this is just an example - you may make your load/save state(s) much fancier! 
	if(key[KEY_F9]) {
		SaveState("state");
		rest(1000);
	}
	if(key[KEY_F10]) {
		LoadState("state");
		rest(1000);
	}

	// Pause
	if(key[KEY_P]) {
		textout(screen, font, "*PAUSED*", 128, 100, 15);
		rest(500);
		while(!key[KEY_P] && !key[KEY_ENTER] && !key[KEY_SPACE]);
		rest(200);
	}
	
	if(keypressed()) {
		pckey = readkey() >> 8;

		//printf("Key pressed: %d\n", pckey);

		switch(pckey) {
		case KEY_TILDE :		// skip ATARI title
            put6502memory(RTC_LO, 253);
            break;
			
		case KEY_F1	:		// Start button
			cont1.key[12] = 1;
			break;
		case KEY_SLASH :	// Start button PL2
			cont2.key[12] = 1;
			break;
		case KEY_F2	:		// Pause button
		case KEY_PAUSE :	// Pause button alternate
			cont1.key[8] = 1;
			break;
		case KEY_ASTERISK :	// Pause button PL2
			cont2.key[8] = 1;
			break;
		case KEY_F3	:		// Reset button
			cont1.key[4] = 1;
			break;
		case KEY_MINUS_PAD :	// Reset button PL2
			cont2.key[4] = 1;
			break;
		case KEY_ESC :		// Escape to monitor
		case KEY_F4	:		// Monitor
			sprintf(errormsg, "Halt pressed!\n");
			//timerTicks = 20;
			running = 0;
			break;
		case KEY_F5	:		// Star key
		case KEY_MINUS :
			cont1.key[3] = 1;
			break;
		case KEY_F6	:		// Hash button
		case KEY_EQUALS :	// Hash alternate
			cont1.key[1] = 1;
			break;
		case KEY_DEL :		// Star key PL2
			cont2.key[3] = 1;
			break;
		case KEY_ENTER :	// Hash button PL2
			cont2.key[3] = 1;
			break;
		case KEY_F7	:		// visualise sound on/off
			//if(!visualise_sound) visualise_sound = 1;
			//else visualise_sound = 0;
			//rest(200);
			break;
		case KEY_F8 :		// choose controller
			//whichcon = (++whichcon % 3);
			//if(whichcon == 0) textout(screen, font, "Controller is now Keyboard.", 4, 4, 15);
			//if(whichcon == 1) textout(screen, font, "Controller is now Joystick.", 4, 4, 15);
			//if(whichcon == 2) textout(screen, font, "Controller is now Mouse.", 4, 4, 15);
			//rest(1000);
			break;
		case KEY_F11 :		// FPS ON/OFF
			if(fps_on) fps_on = 0;
			else fps_on = 1;
			rest(500);
			break;
		case KEY_F12 :		// Save PCX
			// ALLEGRO DEPENDANT
         	// with Allegro, we cannot save the screen directly.
         	// It has to be copied to a buffer, then saved.
			//blit(screen, buffer, 0, 0, 0, 0, 320, 240);
			//save_pcx("5200dump.pcx", buffer, pal5200);
			//textout(screen, font, "Screen dump saved as 5200dump.pcx", 4, 4, 15);
			//rest(1000);
			break;
		case KEY_0	:
			cont1.key[2] = 1;
			break;
		case KEY_1	:
			cont1.key[15] = 1;
			break;
		case KEY_2	:
			cont1.key[14] = 1;
			break;
		case KEY_3	:
			cont1.key[13] = 1;
			break;
		case KEY_4	:
			cont1.key[11] = 1;
			break;
		case KEY_5	:
			cont1.key[10] = 1;
			break;
		case KEY_6	:
			cont1.key[9] = 1;
			break;
		case KEY_7	:
			cont1.key[7] = 1;
			break;
		case KEY_8	:
			cont1.key[6] = 1;
			break;
		case KEY_9	:
			cont1.key[5] = 1;
			break;
		case KEY_Z	:	// BRK key (side triggers)
			// set BRK interrupt bit to 0
			irqst &= 0x7F;
			// check irqen and do interrupt if bit 7 set
			if(irqen & 0x80) irq6502();
			break;
		} // end switch

	} // end if keypressed
    //clear_keybuf();
}

// Interrupts
void HostEnableInterrupts(void) {
}

void HostDisableInterrupts(void) {
}

// Sound output
void HostProcessSoundBuffer(void) {
	if(sound_on) stream_pokey();
}


// *********************** MAIN ************************************
int main(int argc, char *argv[]) {
	int i;

	printf("Main!\n");

	// Init Platform Stuff
	// init allegro (for gfx and sound)
	printf("Initialising Allegro...\n");
    printf("%s\n", allegro_id);
	i = allegro_init();
	printf("allegro_init() returned %d\n", i);
	if(i) {
		printf("Error initialising Allegro!\n");
		exit(1);
	}
	install_timer();
	install_keyboard();
	install_mouse();
	printf("Initialising sound...\n");
	i = install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
	printf("install_sound() returned %d\n", i);
	if(i) {
		printf("Error initialising sound!\n");
		exit(2);
	}

	// Set up Platform Video Output
	// set up screen mode
	printf("Setting up GFX mode...\n");
	set_color_depth(8);
	i = set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0);
	sprintf(errormsg, "set_gfx_mode() returned %d\n", i);

	// GFX mode must be set up before we call Jum52_Initialise()
	// because it calls Init_gfx(), which sets the palette
	if (Jum52_Initialise() == 0)
	{

		printf("vid address: %8X\n", vid);
	
		// create Allegro graphics buffer, and fiddle it so that it
		// uses memory block allocated by Jum52. 
		gfxdest = create_bitmap(VID_WIDTH, VID_HEIGHT);

		// fiddle
		gfxdest->dat = (void *)vid;
		for(i=0; i<VID_HEIGHT; i++) {
			gfxdest->line[i] = vid + (i * VID_WIDTH);
		}

		printf("gfxdest.dat address: %8X\n", gfxdest->dat);
		printf("gfxdest.line[0] address: %8X\n", gfxdest->line[0]);
                                                                    
		// NTSC or PAL - default to NTSC
		// use cmdline switch for PAL
		videomode = NTSC;
		if((*argv[2] == 'P') || (*argv[2] == 'p')) videomode = PAL;
	
		// install frame counter handler
		LOCK_VARIABLE(frame);
		LOCK_VARIABLE(fcounter);
		LOCK_FUNCTION(mytimerhandler);
		// install timer depending on NTSC or PAL
		if(videomode == NTSC) {
			printf("NTSC mode (60 Hz)\n");
			i = install_int(mytimerhandler, 16);		// 60 Hz
		}
		else {
			printf("PAL mode (50 Hz)\n");
			i = install_int(mytimerhandler, 20);		// 50 Hz
		}
		frame = 0;
		// framespeed = no of CPU "frames" to frames drawn
		// 0 - don't speed limit
		// 1 - throttle to 60 fps (50 fps)
		// 2 - throttle to 30 fps (25 fps)
		framespeed = 1;
		fps = 0;
		fps_on = 0;

		if (Jum52_LoadROM(argv[1]) == 0)
		{

			// hack - enable sound
			sound_on = 1;
			sound_vol = 240;

		    // init sound if enabled
    		if(sound_on) {
    			if(init5200sound()) {
        			printf("init5200sound() failed.\n");
        			//exit(1);
                    sound_on = 0;   // disable sound
				}
    		}

            if(!sound_on) printf("Sound disabled - POKEY emulation not started.\n");

		
			Jum52_Emulate(); 
			
			// Teardown Platform Video Output
			if(sound_on) kill5200sound();
		}
		else {
			printf("Could not load ROM!\n");
		}

		exit(0);
	}	
}

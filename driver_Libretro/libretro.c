
//#include <iostream>
#include <string.h>
#include <stdio.h>

#include "libretro.h"

#include "../global.h"
#include "../osdepend.h"
#include "../gamesave.h"
#include "../5200.h"
#include "../pokey.h"
#include "main.h"

#define LOGI printf
int RLOOP=1;

char Key_Sate[512];
char Key_Sate2[512];
char RPATH[512];
char RETRO_DIR[512];
const char *retro_save_directory;
const char *retro_system_directory;
const char *retro_content_directory;
char retro_system_conf[512];

int retrow=320;
int retroh=240;

int pauseg=0;

#define AUDIOBUFLENGTH	4096
#define SND_NTSC 735
#define SND_PAL 800

uint8 audioBuffer[4096];
signed short soundbuf[1024*2];
uint8 *gfxdest;

bool opt_analog=false;

uint32_t videoBuffer[320*240];

static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_batch_t audio_batch_cb;
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { (void)cb; }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

extern void exec6502fast(int n);

void texture_init(){
        memset(videoBuffer, 0, sizeof(videoBuffer));
} 

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   struct retro_variable variables[] = {
      {
         "jum52_analog","Use Analog; OFF|ON",
      },
      { NULL, NULL },
   };

   cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);
}

static void update_variables(void)
{
   struct retro_variable var = {0};

 
   var.key = "jum52_analog";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      fprintf(stderr, "value: %s\n", var.value);
      if (strcmp(var.value, "OFF") == 0)
         opt_analog = false;
      if (strcmp(var.value, "ON") == 0)
         opt_analog = true;

        fprintf(stderr, "[libretro-test]: Analog: %s.\n",opt_analog?"ON":"OFF");
   }

}

void update_input()
{
        if (pauseg==-1)
        	return;
	int i;
	for (int i = 0; i < 16; i++)
		{
		cont1.key[i] = 0;
		cont2.key[i] = 0;
		}

	cont1.trig = 0;
	cont2.trig = 0;
        cont1.side_button = 0;
        cont2.side_button = 0;

	// joystick is off unless pressed below
	cont1.left = cont1.right = cont1.up = cont1.down = 0;
	cont2.left = cont2.right = cont2.up = cont2.down = 0;

    	input_poll_cb();

//player 1

	if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)) cont1.left = 1;
	else if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT)) cont1.right = 1;

	if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)) cont1.up = 1;
	else if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)) cont1.down = 1;

	if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)) cont1.trig = 1;
    
        if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)){
		cont1.side_button = 1;
		// set BRK interrupt bit to 0
		irqst &= 0x7F;
		// check irqen and do interrupt if bit 7 set
		if(irqen & 0x80)
			irq6502();
	
    	}

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START))// Start button
			cont1.key[12] = 1;

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT))// pause button
			cont1.key[8] = 1;

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3))// reset button
			cont1.key[4] = 1;
    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3))// break button
			cont1.key[0] = 1;

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L))// star button
			cont1.key[3] = 1;

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R))// hash button
			cont1.key[1] = 1;

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X))// 0 button
			cont1.key[2] = 1;

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y))// 1 button
			cont1.key[15] = 1;

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2))// 2 button
			cont1.key[14] = 1;

    	if(	input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2))// 3 button
			cont1.key[13] = 1;

//player 2 

	if(input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)) cont2.left = 1;
	else if(input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT)) cont2.right = 1;

	if(input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)) cont2.up = 1;
	else if(input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)) cont2.down = 1;

	if(input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)) cont2.trig = 1;
    
        if(input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)){
		cont2.side_button = 1;
		// set BRK interrupt bit to 0
		irqst &= 0x7F;
		// check irqen and do interrupt if bit 7 set
		if(irqen & 0x80)
			irq6502();
	
    	}

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START))// Start button
			cont2.key[12] = 1;

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT))// pause button
			cont2.key[8] = 1;

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3))// reset button
			cont2.key[4] = 1;
    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3))// break button
			cont2.key[0] = 1;

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L))// star button
			cont2.key[3] = 1;

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R))// hash button
			cont2.key[1] = 1;

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X))// 0 button
			cont2.key[2] = 1;

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y))// 1 button
			cont2.key[15] = 1;

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2))// 2 button
			cont2.key[14] = 1;

    	if(	input_state_cb(1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2))// 3 button
			cont2.key[13] = 1;


	// These may be set to 1. The core handles clearing them.
	// [BREAK]  [ # ]  [ 0 ]  [ * ]
	// [RESET]  [ 9 ]  [ 8 ]  [ 7 ]
	// [PAUSE]  [ 6 ]  [ 5 ]  [ 4 ]
	// [START]  [ 3 ]  [ 2 ]  [ 1 ]


     Key_Sate[RETROK_0] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_0) ? 0x80 : 0;
     if(Key_Sate[RETROK_0] && Key_Sate2[RETROK_0] == 0){
		cont1.key[2] = 1;
		Key_Sate2[RETROK_0]=1;
     }
     else if(!Key_Sate[RETROK_0] && Key_Sate2[RETROK_0] == 1)Key_Sate2[RETROK_0]=0;


     Key_Sate[RETROK_1] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_1) ? 0x80 : 0;
     if(Key_Sate[RETROK_1] && Key_Sate2[RETROK_1] == 0){
		cont1.key[15] = 1;
		Key_Sate2[RETROK_1]=1;
     }
     else if(!Key_Sate[RETROK_1] && Key_Sate2[RETROK_1] == 1)Key_Sate2[RETROK_1]=0;

     Key_Sate[RETROK_2] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_2) ? 0x80 : 0;
     if(Key_Sate[RETROK_2] && Key_Sate2[RETROK_2] == 0){
		cont1.key[14] = 1;
		Key_Sate2[RETROK_2]=1;
     }
     else if(!Key_Sate[RETROK_2] && Key_Sate2[RETROK_2] == 1)Key_Sate2[RETROK_2]=0;

     Key_Sate[RETROK_3] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_3) ? 0x80 : 0;
     if(Key_Sate[RETROK_3] && Key_Sate2[RETROK_3] == 0){
		cont1.key[13] = 1;
		Key_Sate2[RETROK_3]=1;
     }
     else if(!Key_Sate[RETROK_3] && Key_Sate2[RETROK_3] == 1)Key_Sate2[RETROK_3]=0;

     Key_Sate[RETROK_4] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_4) ? 0x80 : 0;
     if(Key_Sate[RETROK_4] && Key_Sate2[RETROK_4] == 0){
		cont1.key[11] = 1;
		Key_Sate2[RETROK_4]=1;
     }
     else if(!Key_Sate[RETROK_4] && Key_Sate2[RETROK_4] == 1)Key_Sate2[RETROK_4]=0;

     Key_Sate[RETROK_5] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_5) ? 0x80 : 0;
     if(Key_Sate[RETROK_5] && Key_Sate2[RETROK_5] == 0){
		cont1.key[10] = 1;
		Key_Sate2[RETROK_5]=1;
     }
     else if(!Key_Sate[RETROK_5] && Key_Sate2[RETROK_5] == 1)Key_Sate2[RETROK_5]=0;

     Key_Sate[RETROK_6] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_6) ? 0x80 : 0;
     if(Key_Sate[RETROK_6] && Key_Sate2[RETROK_6] == 0){
		cont1.key[9] = 1;
		Key_Sate2[RETROK_6]=1;
     }
     else if(!Key_Sate[RETROK_6] && Key_Sate2[RETROK_6] == 1)Key_Sate2[RETROK_6]=0;

     Key_Sate[RETROK_7] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_7) ? 0x80 : 0;
     if(Key_Sate[RETROK_7] && Key_Sate2[RETROK_7] == 0){
		cont1.key[7] = 1;
		Key_Sate2[RETROK_7]=1;
     }
     else if(!Key_Sate[RETROK_7] && Key_Sate2[RETROK_7] == 1)Key_Sate2[RETROK_7]=0;

     Key_Sate[RETROK_8] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_8) ? 0x80 : 0;
     if(Key_Sate[RETROK_8] && Key_Sate2[RETROK_8] == 0){
		cont1.key[6] = 1;
		Key_Sate2[RETROK_8]=1;
     }
     else if(!Key_Sate[RETROK_8] && Key_Sate2[RETROK_8] == 1)Key_Sate2[RETROK_8]=0;

     Key_Sate[RETROK_9] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_9) ? 0x80 : 0;
     if(Key_Sate[RETROK_9] && Key_Sate2[RETROK_9] == 0){
		cont1.key[5] = 1;
		Key_Sate2[RETROK_9]=1;
     }
     else if(!Key_Sate[RETROK_9] && Key_Sate2[RETROK_9] == 1)Key_Sate2[RETROK_9]=0;

     Key_Sate[RETROK_s] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_s) ? 0x80 : 0;
     if(Key_Sate[RETROK_s] && Key_Sate2[RETROK_s] == 0){
		cont1.key[12] = 1;
		Key_Sate2[RETROK_s]=1;
     }
     else if(!Key_Sate[RETROK_s] && Key_Sate2[RETROK_s] == 1)Key_Sate2[RETROK_s]=0;

     Key_Sate[RETROK_r] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_r) ? 0x80 : 0;
     if(Key_Sate[RETROK_r] && Key_Sate2[RETROK_r] == 0){
		cont1.key[4] = 1;
		Key_Sate2[RETROK_r]=1;
     }
     else if(!Key_Sate[RETROK_r] && Key_Sate2[RETROK_r] == 1)Key_Sate2[RETROK_r]=0;

     Key_Sate[RETROK_b] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_b) ? 0x80 : 0;
     if(Key_Sate[RETROK_b] && Key_Sate2[RETROK_b] == 0){
		cont1.key[0] = 1;
		Key_Sate2[RETROK_b]=1;
     }
     else if(!Key_Sate[RETROK_b] && Key_Sate2[RETROK_b] == 1)Key_Sate2[RETROK_b]=0;

     Key_Sate[RETROK_p] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_p) ? 0x80 : 0;
     if(Key_Sate[RETROK_p] && Key_Sate2[RETROK_p] == 0){
		cont1.key[8] = 1;
		Key_Sate2[RETROK_p]=1;
     }
     else if(!Key_Sate[RETROK_p] && Key_Sate2[RETROK_p] == 1)Key_Sate2[RETROK_p]=0;

     Key_Sate[RETROK_h] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_h) ? 0x80 : 0;
     if(Key_Sate[RETROK_h] && Key_Sate2[RETROK_h] == 0){
		cont1.key[1] = 1;
		Key_Sate2[RETROK_h]=1;
     }
     else if(!Key_Sate[RETROK_h] && Key_Sate2[RETROK_h] == 1)Key_Sate2[RETROK_h]=0;

     Key_Sate[RETROK_t] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_t) ? 0x80 : 0;
     if(Key_Sate[RETROK_t] && Key_Sate2[RETROK_t] == 0){
		cont1.key[3] = 1;
		Key_Sate2[RETROK_t]=1;
     }
     else if(!Key_Sate[RETROK_t] && Key_Sate2[RETROK_t] == 1)Key_Sate2[RETROK_t]=0;

     Key_Sate[RETROK_BACKSPACE] = input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_BACKSPACE) ? 0x80 : 0;
     if(Key_Sate[RETROK_BACKSPACE] && Key_Sate2[RETROK_BACKSPACE] == 0){
		put6502memory(RTC_LO, 253);
		Key_Sate2[RETROK_BACKSPACE]=1;
     }
     else if(!Key_Sate[RETROK_BACKSPACE] && Key_Sate2[RETROK_BACKSPACE] == 1)Key_Sate2[RETROK_BACKSPACE]=0;


}


#if 0
static void keyboard_cb(bool down, unsigned keycode, uint32_t character, uint16_t mod)
{
}
#endif

/************************************
 * libretro implementation
 ************************************/

//static struct retro_system_av_info g_av_info;

void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
	info->library_name = "jum52";
	info->library_version = "1.3";
	info->need_fullpath = true;
	info->valid_extensions = "bin|a52";
}


void retro_get_system_av_info(struct retro_system_av_info *info)
{
//FIXME handle vice PAL/NTSC
   struct retro_game_geometry geom = { retrow, retroh,retrow, retroh ,4.0 / 3.0 };
   struct retro_system_timing timing = { 60.0, 44100.0 };

   info->geometry = geom;
   info->timing   = timing;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
    (void)port;
    (void)device;
}

size_t retro_serialize_size(void)
{
	return 0;
}

bool retro_serialize(void *data, size_t size)
{
    return false;
}

bool retro_unserialize(const void *data, size_t size)
{
    return false;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
    (void)index;
    (void)enabled;
    (void)code;
}

bool retro_load_game(const struct retro_game_info *info)
{    
	const char *full_path;

    	full_path = info->path;

	strcpy(RPATH,full_path);

	printf("LOAD EMU\n");

	memset(audioBuffer, 128, AUDIOBUFLENGTH); 
	options.videomode = NTSC;
	Jum52_Initialise();
	gfxdest = vid;
	Jum52_LoadROM( RPATH );
        clearSampleEvents();

    	return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
    (void)game_type;
    (void)info;
    (void)num_info;
    return false;
}

void retro_unload_game(void)
{
     pauseg=0;
}

unsigned retro_get_region(void)
{
    return RETRO_REGION_NTSC;
}

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void *retro_get_memory_data(unsigned id)
{
    return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
    return 0;
}

void retro_init(void)
{
   const char *system_dir = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_dir) && system_dir)
   {
      // if defined, use the system directory			
      retro_system_directory=system_dir;		
   }		   

   const char *content_dir = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_CONTENT_DIRECTORY, &content_dir) && content_dir)
   {
      // if defined, use the system directory			
      retro_content_directory=content_dir;		
   }			

   const char *save_dir = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &save_dir) && save_dir)
   {
      // If save directory is defined use it, otherwise use system directory
      retro_save_directory = *save_dir ? save_dir : retro_system_directory;      
   }
   else
   {
      // make retro_save_directory the same in case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY is not implemented by the frontend
      retro_save_directory=retro_system_directory;
   }

   if(retro_system_directory==NULL)sprintf(RETRO_DIR, "%s\0",".");
   else sprintf(RETRO_DIR, "%s\0", retro_system_directory);

   sprintf(retro_system_conf, "%s/jum52.cfg\0",RETRO_DIR);

   LOGI("Retro SYSTEM_DIRECTORY %s\n",retro_system_directory);
   LOGI("Retro SAVE_DIRECTORY %s\n",retro_save_directory);
   LOGI("Retro CONTENT_DIRECTORY %s\n",retro_content_directory);


    enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
    if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
    {
        fprintf(stderr, "Pixel format XRGB8888 not supported by platform, cannot use.\n");
        exit(0);
    }

   struct retro_input_descriptor inputDescriptors[] = {
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2, "R2" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2, "L2" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3, "R3" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3, "L3" }
	};
	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, &inputDescriptors);

/*
    struct retro_keyboard_callback cbk = { keyboard_cb };
    environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &cbk);
*/
 	memset(Key_Sate,0,512);
 	memset(Key_Sate2,0,512);

  	update_variables();

}

void retro_deinit(void)
{
//	QuitEmulator();
        printf("Retro DeInit\n");
}

void retro_reset(void)
{
}

void HostLog(char *s)
{
	printf("%s\n", s);
}

void HostPrepareForPaletteSet(void) {
}

void HostSetPaletteEntry(uint8 entry, uint8 red, uint8 green, uint8 blue) {

}
void HostRefreshPalette(void) {
}

void HostBlitVideo(void) {
	RLOOP=0;
}
void HostDoEvents(void) {

}
// Sound output
void HostProcessSoundBuffer(void) {

	Pokey_process(audioBuffer, AUDIOBUFLENGTH);

	// render "voice" buffer if necessary
	if(options.voice)
		renderMixSampleEvents(audioBuffer, AUDIOBUFLENGTH);

}

void retro_run(void)
{       
	int i;
	char col;

	RLOOP=1;

	exec6502fast(2000000000);

	update_input();

	if(pauseg!=-1){

		int vol64;

		vol64 = (options.volume * 63) / 100;
		uint8* ptr=(uint8*)&soundbuf[0];
		for (i = 0; i < SND_NTSC*4; i++){
			short value = audioBuffer[i];
			value -= 128;
			value *= 64;
			ptr[i*2] = value & 0xFF;
			ptr[i*2+1] = value >> 8;
		}
		
		audio_batch_cb(soundbuf,SND_NTSC);
	}

	
	uint32 *pScr= (uint32 *)videoBuffer;
	uint8 *pLine;
	int x,y;

	for(y=0; y<240; y++)
	{
		pLine = vid + ((y+8) * VID_WIDTH);  // skip top 8 lines
		for(x=0; x<320; x++)
		{
			*pScr++ = colourtable[*pLine++];
			
		}
	}

        video_cb(videoBuffer, retrow, retroh, retrow << 2);

}


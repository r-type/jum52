# Makefile for PSP version of Jum52
#
TARGET = jum52
OBJS = main.o 6502dis.o ../5200gfx.o ../52emu.o ../6502.o ../font5200.o ../pokey.o ../crc32.o ../gamesave.o

PSPBIN = $(PSPDEV)/psp/bin

INCDIR = 
#CFLAGS = -DPSP -D_DEBUG -O2 -G0 -Wall $(shell $(PSPBIN)/sdl-config --cflags)
CFLAGS = -DPSP -O2 -G0 -Wall $(shell $(PSPBIN)/sdl-config --cflags)
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = 
LDFLAGS = 
#LIBS = -lm $(shell $(PSPBIN)/sdl-config --libs) -logg -lvorbis -lvorbisidec -lSDL_mixer
LIBS = -lm $(shell $(PSPBIN)/sdl-config --libs) -lSDLmain -lSDL -lm -lstdc++

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Jum52 v1.1
PSP_EBOOT_ICON = icon.png
PSP_EBOOT_ICON1 = icon1.png
PSP_EBOOT_PIC1 = pic1.png
#PSP_EBOOT_SND0 = snd0.at3

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


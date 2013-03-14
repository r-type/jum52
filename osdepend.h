// osdepend.h
// These are OS or platform dependant functions that need to be
// implemented by every OS or platform "driver"

#ifndef _OSDEP_H
#define _OSDEP_H

void HostLog( char *s );

void HostPrepareForPaletteSet(void);

// Set palette entries - assumes 0-255 for each entry
void HostSetPaletteEntry(uint8 entry, uint8 red, uint8 green, uint8 blue);

// Required for all the previous changes to take effect
void HostRefreshPalette(void);

// Actual video copy
void HostBlitVideo(void);

// Check key status
void HostDoEvents(void);

// Interrupts
void HostEnableInterrupts(void);
void HostDisableInterrupts(void);

// Sound output
void HostProcessSoundBuffer(void);

#endif
Jum's A5200 Emulator, PSP/SDL Version 1.1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

28 February 2010

This is an Atari 5200 emulator for Sony PSP.

The 6502 CPU emulator source is heavily based on a distribution
by Neil Bradley. The POKEY sound emulator is a modified version 
of Ron Fries POKEY emulator.
The rest is by me.


Obligatory Copyright Notice:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jum's A5200 Emulator is copyright 1999-2010 by James Higgs.
POKEY Sound is copyright 1996 by Ron Fries.

Jum's A5200 Emulator is free as long as it is not used in a commercial
matter and not altered in any way. The contents of this archive should
not be added to or changed in any way. 

I maintain the right to forbid the use of the emulator at
any time. I am not responsible for any damage caused by the use
of this program. This program is distributed "as-is". I make no
guarantees as to it's accuracy, performance, or compatibility with
the user's hardware.

Jum's A5200 Emulator ("Jum52") is not to be included in commercial
emulator packages of any sort.


******************************************************************
	DO NOT ASK ME FOR ROM OR CARTRIDGE IMAGES !!!
******************************************************************

Special notes for PSP version:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- "voice" emulation (Bezerk/Baseball/etc) not working
- Mouse and MousePaddle control modes not working
- Audio working but sounds crappy.
- Frameskip is used to get full speed (60 fps).



Compatibility:
~~~~~~~~~~~~~~

Most carts will run.

These games give problems:
- Decathlon
- Mr. Do's Castle (corrupt rom dump)
- Quest for Quintana Roo
- Rescue on Fractalus
- Buck Rogers
- Gyruss




PSP Controls:
~~~~~~~~~~~~~

SELECT          - Options Menu
DPAD / Analog   - 5200 Joystick
X               - 5200 Fire 1
O               - 5200 Fire 2
LSB	        - 5200 *
RSB	        - 5200 #
Start           - 5200 Start
Hold            - 5200 Pause
SQUARE + Up 	- 5200 Keypad 1
SQUARE + Right	- 5200 Keypad 2
SQUARE + Down	- 5200 Keypad 3
SQUARE + Left	- 5200 Keypad 4
SQUARE + LSB	- 5200 Keypad 5
SQUARE + RSB	- 5200 Keypad 6
SQUARE + TRI	- 5200 Keypad 7
SQUARE + O	- 5200 Keypad 8
SQUARE + X	- 5200 Keypad 9
TRI		- 5200 Keypad 0



Joystick Modes:
~~~~~~~~~~~~~~

From the Options Menu, select joystick mode by toggling "Controller" (press left or right arrow key while on the Controller option). You can toggle between using the DPAD or the Analog Nub as the 5200 joysick.

Pengo control mode is automatically activated when Pengo cart is loaded.

Many games do "auto-calibrating" while you play. Moving the joystick handle in a big circle while chanting "work dammit" usually gets it working OK. Also it sometimes helps if you leave the analog stick in the central position when starting a game.

If you activate joystick mode, and the game does not control as
expected, then try reloading the game.



Getting started:
~~~~~~~~~~~~~~~

You will need:
- A PSP that can run homebrew software (ie: a PSP with custom firmware).
- Some 16k or 32k Atari 5200 cartridge images ("roms").

1. Copy the jum52 folder to the PSP\GAME folder on your PSP.
2. Copy the cartridge images into the folder PSP\GAME\jum52 on your PSP.
   *** NB: Catridge images MUST have extension .BIN ***
3. Run Jum52 from your PSP's XMB.
4. Have fun!



Configuring Jum52:
~~~~~~~~~~~~~~~~~

If you get tired of having to change the options in Jum52 every time you 
run it, then you probably want to set the default options in the 
jum52.cfg file:

1. Make a copy of jum52.cfg (call it "jum52.cfg.bak" or something).
2. Edit jum52.cfg with Notepad.
3. Set the options as you prefer (valid options are in brackets).
4. Save jum52.cfg.

NB: "scale" and "fullscreen" options in jum52.cfg have no effect in the
    PSP version of Jum52.

You can also add mappings for "unknown" 16k roms (eg: if you're writing 
a homebrew game and you want to test it in Jum52 :). In this case, take 
a look at the sample mappings in jum52.cfg. you can get the crc of the 
rom by running it in Jum52, then looking in the 5200.log file. 




******************************************************************
	DO NOT ASK ME FOR ROM OR CARTRIDGE IMAGES !!!
******************************************************************



FAQ:
~~~~

1. Q: It doesn't run on my Mac
   A: Get the Mac version from www.bannister.org
      (It's also available for Windows, PS2, QNX and BeOS)

2. Q: It's crap. There's no blah blah yadda yadda ...
   A: It's free. Waddaya expect?

3. Q: It's so sssslllllooooowwwwwwwww...........
   A: Your PC is crap.
   A: Set 'scale=1' in jum52.cfg
   A: Run your Windows emulator on a faster PC.

4. Q: I have trouble getting my joystick to work.
   A: It happens to even the best of men.
   A: Make sure that it's set up properly in Windows Control Panel.

5. Q: I don't hear any sound.
   A: Cut down on the heavy metal.
   A: Turn up the volume REALLY loud.
   A: Check the volume setting in the Jum52's Options Menu
   A: Check that 'audio=on' is in jum52.cfg

6. Q: Game X doesn't work.
   A: It may be a corrupt/bad ROM image (there are many).
   A: If you can't get past the start screen in the game,
      then Jum52 just doesn't handle that game (yet).
   A: If it's a 16k rom, try adding a mapping for it in jum52.cfg

8. Q: When I select "Load Game", nothing is listed.
   A: Copy your Atari 5200 game roms (with a .BIN file extension) to 
      the same folder as Jum52.

9. Q: Where can I get ROMZ? (plead/whine/grovel/demand)
   A: Learn to use a search engine, or something.



Troubleshooting and Comments:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Email:  james7780@yahoo.com

1. Examine the 5200.log file for errors.
2. RTFM ("Getting started" above)
3. Intelligent questions are welcome.
4. Constructive comments are appreciated (especially comments on
   how the emulator differs from the real thing).


Future Features:
~~~~~~~~~~~~~~~~

1. Better audio.
2. Faster.
3. Better controller support.
4. Whatever you can suggest? 


Credits:
~~~~~~~~
Thanks to:
Dan Boris (author of VSS and V7800) for infos.
Ron Fries (for POKEY emu).
Neil Bradley for 6502 emu.
Sherwood for helpful comments and other stuff.
Christopher Durante for useful input.
Richard Bannister for cross-platform conversion and Mac version.
John Swiderski.
Calamari (5200Bas).
5200 homebrew devers.
Other people who helped, contributed, cajoled or complained.


# MSP432 Function Generator
A function generator for the MSP432.

## Behavior Description
When powered on, the system will output a 100 Hz square wave. Keys on the keypad can adjust this output.

OPTIONS:
- Keys 1-5: sets output frequency to 100-500Hz, depending on which key is pressed.
- Key 7: outputs a square wave
- Key 8: outputs a sine wave
- Key 9: outputs a sawtooth wave
- Key '\*': decreases duty cycle of square wave in increments of 10% down to 10%
- Key 0: increases duty cycle of square wave in increments of 10% up to 90%
- Key'#': sets the duty cycle to 50% (the default duty cycle)

Keys '\*', '#', and '0' all will affect the next time a square wave is run, if it is not currently running.

All waveforms exhibit a 3 V peak-to-peak value with strictly positive voltage values. This could be changed very easily in the future -- the functions in waveforms.c to generate wave tables are written so that you pass in what voltage you want outputted. It has been hard-coded to 3 for the school assignment this was originally written for.

On the connected LCD display will be displayed which wave is currently running, the frequency at which it is running, and if running a square wave, its duty cycle.

## Hardware Used
1. MSP432401R
2. Generic 12-Key Keypad (see keypad.h for connections to MSP432)
3. MCP4921 DAC (see spi.h for conecctions to MSP432)
4. 1602A LCD Display (see lcd.h for connections to MSP432)

## The Nitty Gritty (Software Description)
The way this program works is by having an interrupt go off a set amount of times per period wanted. In this code, it is set to 128 divisions, but this is adjustable with WAVE_TABLE_SIZE in waveforms.h.

Every interrupt, an interrupt handler will read the next value from a pointer to a lookup table. This pointer can point to any integer table of size WAVE_TABLE_SIZE.

When the code first runs, three tables are created:
- a square wave table
- a sawtooth wave table
- a sine wave table
This code could easily be expanded to include more wavetypes by just adding another table with this waveform populated in it.

A global pointer points to one of these lists, and indexing into this global pointer is how the MSP432 knows which value to next send to the DAC. To change which wave is being output, just change the pointer to point to that wave's table.

To change the frequency, we just change how often the interrupt goes off, thus not requiring us to mess with the wave tables after creating them.

In the case of changing the duty cycle of the square wave, we currently completely repopulate the table. In the future, it's definitely possible to only modify the spots in the table that will change in order to save some complexity, but for now this function generator is fast enough that this has been left as is.

That's how it works! :)

#### Acknowledgements
Written by Michael Georgariou with input from Connor McKee
Written for Dr. John Oliver's Computer Architecture class

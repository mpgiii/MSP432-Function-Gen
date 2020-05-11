/*
 * LCD.c
 *
 *  Created on: Apr 16, 2020
 *      Author: Michael Georgariou & Connor McKee
 */
#include "msp.h"
#include "LCD.h"
#include "delay.h"
#include <stdio.h>


void write_string_to_LCD(char* string) {
    int i;

    /* loop through the inputted string, writing each character, until we hit
     * the null terminator. */
    for (i = 0; string[i] != '\0'; i++) {
        LCD_nibble_write(string[i]);
    }
}

void LCD_write_command(uint8_t command){

    /* clear out the data bits */
    LCD0->OUT &= ~(DB7 | DB6 | DB5 | DB4);
    /* set the bits high that correspond to the inputted command */
    LCD0->OUT |= (0xF0 & command);
    /* set RW and RS low, to input command */
    LCD0->OUT &= ~RW;
    LCD0->OUT &= ~RS;

    /* pulse the enable bit */
    delay_us(1000);
    LCD0->OUT |= EN;
    delay_us(1000);
    LCD0->OUT &= ~EN;

    /* figure the delay for next command/write */
    delay_us(2000);
}

void LCD_nibble_write_command(uint8_t command) {
    /* write the highest four bits, then the lowest four bits. */
    LCD_write_command(command & 0xF0);
    delay_us(1000);
    LCD_write_command(command << 4);
    delay_us(1000);
}

void LCD_write(uint8_t data){

    /* clear out the data bits */
    LCD0->OUT &= ~(DB7 | DB6 | DB5 | DB4);
    /* set the bits high that correspond to the inputted data */
    LCD0->OUT |= (0xF0 & data);
    /* set RW low and RS high, to signify data input */
    LCD0->OUT &= ~RW;
    LCD0->OUT |= RS;

    /* pulse the enable bit */
    delay_us(1000);
    LCD0->OUT |= EN;
    delay_us(1000);
    LCD0->OUT &= ~EN;

    /* delay for next command/data */
    delay_us(2000);
}

void LCD_nibble_write(uint8_t data){
    /* write the highest four bits, then the lowest four bits. */
    LCD_write(data & 0xF0);
    delay_us(50);
    LCD_write(data << 4);
    delay_us(50);
}

void LCD_go_to_line(int line)
{
    /* set the DDRAM address to start writing at the inputted line. */
    if (line == 2) {
        LCD_nibble_write_command(DDRAM_ADDRESS_SET | 0x28);
    }
    else {
        LCD_nibble_write_command(DDRAM_ADDRESS_SET | 0x00);
    }
}

void LCD_reset()
{
    /* pretty self explanatory! */
    LCD_clear();
    LCD_home();
}

/* This function returns the cursor to the upper left corner */
void LCD_home()
{
    LCD_nibble_write_command(DISPLAY_CURSOR_HOME);
}

/* This function will clear the screen */
void LCD_clear()
{
    LCD_nibble_write_command(CLEAR_DISPLAY);
}

/* This function will turn off the cursor */
void LCD_turn_off_cursor() {
    LCD_nibble_write_command(DISPLAY_CONTROL | DISPLAY_ON);
}

/* This function will turn on the cursor */
void LCD_turn_on_cursor() {
    LCD_nibble_write_command(DISPLAY_CONTROL | DISPLAY_ON |
                             CURSOR_DISPLAY_ON | CURSOR_BLINK_ON);
}

void LCD_init(){
//    LCD0->SEL1 &= ~(0xFF); /* Configure P4. as simple I/O */
//    LCD0->SEL0 &= ~(0xFF);

    /* configure all of the LCD pins as output */
    LCD0->DIR |= 0xFF;

    /* delay before initialization, for boot time */
    delay_us(100000);

    /* wake up! */
    LCD_write_command(0x00);
    LCD_write_command(0x00);
    LCD_write_command(0x00);
    LCD_write_command(0x30);
    delay_us(100000);
    LCD_write_command(0x30);
    delay_us(100000);
    LCD_write_command(0x20);
    delay_us(100000);

    /* set function set for dual line */
    LCD_nibble_write_command(FUNCTION_SET | DL_NUMBER);

    /* turn on the display, with a blinking cursor */
    LCD_nibble_write_command(DISPLAY_CONTROL | DISPLAY_ON |
                            CURSOR_DISPLAY_ON | CURSOR_BLINK_ON);

    /* clear the display */
    LCD_nibble_write_command(CLEAR_DISPLAY);

    /* change entry mode to increment */
    LCD_nibble_write_command(ENTRY_MODE_SET | INCREMENT_DECREMENT);

}

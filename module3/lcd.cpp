/**
\file lcd.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

LCD controls for Project 2 Module 3 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "pin_mapping.h"
#include "lcd.h"

///40 microsecond delay
#define LCD_DELAY_SHORT_US (40)

///2,000 microsecond delay
#define LCD_DELAY_LONG_US (2000)

///50,000 microsecond delay
#define LCD_DELAY_POWER_US (50000)

///1 second delay
#define LCD_DELAY_SPLASH_S (1)

///screen size of 16 characters 
#define LCD_SCREEN_SIZE (16)

///bus of wires connecting the LCD to the STM NUCLEO F401RE
BusOut lcd_bus(LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

///set the pin connected to the LCD's "RS" pin as a digital output
DigitalOut lcd_rs(LCD_RS);
/*
RS is high when we write to the display (displaying something on the screen)
RS is low when we write instructions to the LCD
*/

///set the pin connected to the LCD's "E" pin as a digital output
DigitalOut lcd_e(LCD_E);
/*
E acts as a mutex of a sort
E needs to be set high before data is written to the LCD
and then it needs to be set low after the data writing is done
*/

/**
* Name: lcd_write <br>
* Description: this function writes data to the LCD <br>
* @param [in] uint8_t data
* @param [out] none
*/
void lcd_write(uint8_t data)
{
    lcd_e = 1; //set E high
    lcd_bus = data; //send the data
    lcd_e = 0; //set E low
}

/**
* Name: lcd_write_ins <br>
* Description: this function writes instructions to the LCD <br>
* @param [in] uint8_t data, uint32_t delay
* @param [out] none
*/
void lcd_write_ins(uint8_t data, uint32_t delay)
{
    lcd_rs = 0; //Set RS low (needs to be low to write instructions)
    lcd_write((data >> 4) & 0xf); //send four bits of data to the LCD, shifting these bits into the lowest 4 bits on the LCD and masking off the remaining bits
    lcd_write(data & 0xf); //send the remaining four bits mask off the highest 4 bits again (possible redundancy)
    wait_us(delay); //wait the specified number of microseconds
}

/**
* Name: lcd_write_char <br>
* Description: this function writes characters to the LCD <br>
* @param [in] uint8_t data
* @param [out] none
*/
void lcd_write_char(uint8_t data)
{
    lcd_rs = 1; //Set RS high (needs to be high to write data)
    lcd_write(data >> 4); //send four bits of data to the LCD, shifting these bits into the lowest 4 bits on the LCD
    lcd_write(data); //send the remaining four bits to the LCD
    wait_us(LCD_DELAY_SHORT_US); //wait for 40 microseconds
}

/**
* Name: lcd_init <br>
* Description: this function initializes the LCD before we use it <br>
* @param [in] none
* @param [out] none
*/
void lcd_init()
{
    wait_us(LCD_DELAY_POWER_US); //wait for 50,000 microseconds
    
    lcd_rs = 0; //set RS low (needs to be low to write instructions)
    lcd_e = 0; //set E low (needs to be low to prevent data being written)
    
    lcd_write_ins(0x20, LCD_DELAY_SHORT_US); //put LCD in 4 wire mode
    lcd_write_ins(0x20, LCD_DELAY_SHORT_US); //put LCD in 1 line mode with 5x8 font
    lcd_write_ins(0x0c, LCD_DELAY_SHORT_US); //display on, cursor off
    lcd_write_ins(0x01, LCD_DELAY_LONG_US); //clear display
    lcd_write_ins(0x06, LCD_DELAY_SHORT_US); //upon character entry the cursor shifts to the right (in front of the character)
}

/**
* Name: lcd_write_str <br>
* Description: this function writes strings to the LCD <br>
* @param [in] char * data
* @param [out] none
*/
void lcd_write_str(char * data)
{
    char * curr_char = data; //create a string of characters and set it equal to the data we want to send
    while(*curr_char != 0) // while we are not at the end of the string
    {
        lcd_write_char(*curr_char); //write the current character to the screen
        curr_char++; //increment to the next character in the string
    }
}

/**
* Name: lcd_printf <br>
* Description: this function writes strings to the LCD <br>
* @param [in] const char * format, ...
* @param [out] none
*/
void lcd_printf(const char * format, ... )
{
    char buffer[LCD_SCREEN_SIZE + 1];
    
    va_list args; //object that contains any extra arguments passed into the function after "format"
    va_start(args, format); //initialize the object, put the additional arguments in the object
    vsprintf(buffer, format, args); //put both format and the args object into the local buffer variable for printing (below)
    va_end(args); //end the object
    
    lcd_clear(); //clear the LCD
    lcd_write_str(buffer); //prints the buffer created above to the LCD
}

/**
* Name: lcd_clear <br>
* Description: this function clears the LCD display <br>
* @param [in] none
* @param [out] none
*/
void lcd_clear()
{
    lcd_write_ins(0x01, LCD_DELAY_LONG_US);
}


/**
* Name: lcd_splash <br>
* Description: this function prints the splash screen to the LCD (typically called immediately after initialization <br>
* @param [in] none
* @param [out] none
*/
void lcd_splash()
{
    lcd_printf("Hello World!"); //print splash screen message
    wait(LCD_DELAY_SPLASH_S); //wait
    lcd_clear(); //clear the LCD
}
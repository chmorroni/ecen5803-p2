/**
\file lcd.h

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

LCD function prototypes for Project 2 Module 3 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#ifndef __LCD_H__
#define __LCD_H__

#include "mbed.h"
#include "pin_mapping.h"

void lcd_init();
void lcd_clear();
void lcd_set_cursor(uint8_t pos);
void lcd_write_char(uint8_t data);
void lcd_write_str(char * data);
void lcd_printf(const char * format, ... );
void lcd_splash();

#endif

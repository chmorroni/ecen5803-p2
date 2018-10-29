
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

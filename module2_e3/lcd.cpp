
#include "pin_mapping.h"
#include "lcd.h"

#define LCD_DELAY_SHORT_US (40)
#define LCD_DELAY_LONG_US (2000)
#define LCD_DELAY_POWER_US (50000)

#define LCD_SCREEN_SIZE (16)

BusOut lcd_bus(LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
DigitalOut lcd_rs(LCD_RS);
DigitalOut lcd_e(LCD_E);

void lcd_write(uint8_t data)
{
    lcd_e = 1;
    lcd_bus = data;
    lcd_e = 0;
}

void lcd_write_ins(uint8_t data, uint32_t delay)
{
    lcd_rs = 0;
    lcd_write((data >> 4) & 0xf);
    lcd_write(data & 0xf);
    wait_us(delay);
}

void lcd_write_char(uint8_t data)
{
    lcd_rs = 1;
    
    lcd_write(data >> 4);
    lcd_write(data);
    wait_us(LCD_DELAY_SHORT_US);
}

void lcd_init()
{
    wait_us(LCD_DELAY_POWER_US);
    
    lcd_rs = 0;
    lcd_e = 0;
    
    lcd_write_ins(0x20, LCD_DELAY_SHORT_US);
    lcd_write_ins(0x20, LCD_DELAY_SHORT_US);
    lcd_write_ins(0x0c, LCD_DELAY_SHORT_US);
    lcd_write_ins(0x01, LCD_DELAY_LONG_US);
    lcd_write_ins(0x06, LCD_DELAY_SHORT_US);
}

void lcd_write_str(char * data)
{
    char * curr_char = data;
    while(*curr_char != 0)
    {
        lcd_write_char(*curr_char);
        curr_char++;
    }
}

void lcd_printf(const char * format, ... )
{
    char buffer[LCD_SCREEN_SIZE + 1];
    
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    lcd_clear();
    lcd_write_str(buffer);
}

void lcd_clear()
{
    lcd_write_ins(0x01, LCD_DELAY_LONG_US);
}

void lcd_set_cursor(uint8_t pos)
{
    uint8_t ins = 0x80 | (pos & 0x7f);
    lcd_write_ins(ins, LCD_DELAY_LONG_US);
}


#include "mbed.h"
#include "lcd.h"
#include "temperature.h"
#include "uart.h"

#define PERIOD_PRINT_TEMP_S (1)

Ticker timer_print_temp;

void print_temp()
{
    float temp = temp_measure();
    uart_printf("Current temperature: %5.2f deg C\r\n", temp);
    lcd_printf("Temp: %5.2f%cC", temp, 0xDF);
}

void lcd_splash()
{
    lcd_printf("Hello World!");
    wait(2);
    lcd_clear();
}

int main()
{   
    uart_init();
    uart_printf("Hello mbed!\r\n");
    
    lcd_init();
    
    temp_init();
    
    lcd_splash();
    
    //timer_print_temp.attach(print_temp, PERIOD_PRINT_TEMP_S);
    
    while(1)
    {
      print_temp();
      wait(1);  
    }
}

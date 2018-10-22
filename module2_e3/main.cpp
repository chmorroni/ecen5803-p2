#include "mbed.h"

#define LCD_MOSI_PIN (PA_7)
#define LCD_SCK_PIN (PA_5)
#define LCD_CS_PIN (PA_6)
#define TEMP_SCL_PIN (PB_8)
#define TEMP_SDA_PIN (PB_9)
#define USB_TX_PIN (PA_2)
#define USB_RX_PIN (PA_3)

Ticker tick;
Serial pc(USB_TX_PIN, USB_RX_PIN);
NHD_0216HZ lcd(LCD_CS_PIN, LCD_MOSI_PIN, LCD_SCK_PIN);
I2C temp(TEMP_SDA_PIN, TEMP_SCL_PIN);

int main()
{
    while(1);
}

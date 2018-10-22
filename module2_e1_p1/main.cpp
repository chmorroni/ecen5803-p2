#include "mbed.h"

#define BUTTON0_PIN (PA_10)
#define BUTTON1_PIN (PB_3)
#define BUTTON2_PIN (PB_5)
#define BUTTON3_PIN (PB_4)
#define LD2_PIN (LED1)
#define BB_LED_PIN (PA_9)

BusOut leds(LD2_PIN, BB_LED_PIN);
BusIn buttons(BUTTON3_PIN, BUTTON2_PIN, BUTTON1_PIN, BUTTON0_PIN);

int main()
{
    while(1)
    {
        if(~buttons & 0x8)
            leds = leds | 0x1;
        if(~buttons & 0x4)
            leds = leds & ~0x1;
        if(~buttons & 0x2)
            leds = leds | 0x2;
        if(~buttons & 0x1)
            leds = leds & ~0x2;
    }
}

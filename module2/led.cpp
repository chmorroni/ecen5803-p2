
#include "mbed.h"
#include "pin_mapping.h"
#include "led.h"

DigitalOut led_ld2(LED_LD2_PIN);
DigitalOut led_external(LED_EXTERNAL_PIN);

void led_ld2_on()
{
    led_ld2 = true;
}

void led_ld2_off()
{
    led_ld2 = false;
}

void led_external_on()
{
    led_external = true;
}

void led_external_off()
{
    led_external = false;
}
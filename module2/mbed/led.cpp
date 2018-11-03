/**
\file led.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

LED controls for Project 2 Module 2 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "pin_mapping.h"
#include "led.h"

///set the on-board LED on the STM NUCLEO F401RE as a digital output
DigitalOut led_ld2(LED_LD2_PIN);

///set the LED on our breadboard as a digital output
DigitalOut led_external(LED_EXTERNAL_PIN);

/**
* Name: led_ld2_on <br>
* Description: this function turns on the on-board LED on the STM NUCLEO F401RE <br>
* @param [in] none
* @param [out] none
*/
void led_ld2_on()
{
    led_ld2 = true;
}

/**
* Name: led_ld2_off <br>
* Description: this function turns off the on-board LED on the STM NUCLEO F401RE <br>
* @param [in] none
* @param [out] none
*/
void led_ld2_off()
{
    led_ld2 = false;
}

/**
* Name: led_external_on <br>
* Description: this function turns on the external (breadboard) LED <br>
* @param [in] none
* @param [out] none
*/
void led_external_on()
{
    led_external = true;
}

/**
* Name: led_external_off <br>
* Description: this function turns off the external (breadboard) LED <br>
* @param [in] none
* @param [out] none
*/
void led_external_off()
{
    led_external = false;
}
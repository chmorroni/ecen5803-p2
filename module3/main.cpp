/**
\file main.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

Main file of Project 2 Module 3 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "rtos.h"
#include "lcd.h"
#include "led.h"
#include "temperature.h"
#include "uart.h"

#include "pin_mapping.h"

///2 second pause used when printing temperatures to the LCD
#define PERIOD_PRINT_TEMP_MS (2000)

///2 second pause used when printing the count variable to the LCD
#define PERIOD_PRINT_COUNT_MS (2000)

///value to set the frequency of the blinking LED to 1Hz
#define PERIOD_LED_BLINK_MS (1000)

///Declare potentiometer 0 (used to adjust LED brightness) as an analog input
AnalogIn brightness_pot(POT0_PIN);

///PWM output for the onboard LED
PwmOut led_ld2_pwm(LED_LD2_PIN);

///Mutex used to "lock" and "unlock" the lcd screen for printing
Mutex lcd_mutex;

/**
* Name: print_temp_thread <br>
* Description: This function prints the temperature to the LCD <br>
* @param [in] none
* @param [out] none
*/
void print_temp_thread()
{
    float temp;
    while(1)
    {
        temp = temp_measure(); //take a temperature measurement
        lcd_mutex.lock(); //"lock" the LCD so no other processes can write to it except this one
        lcd_printf("Temp: %5.2f%cC", temp, 0xDF); //print the temperature reading
        lcd_mutex.unlock(); //"unlock" the LCD so other processes can use it again
        ThisThread::sleep_for(PERIOD_PRINT_TEMP_MS); //pause to allow the text to be displayed on the screen
    }
}

/**
* Name: led_brightness_thread <br>
* Description: This function adjusts the brighness of our external (breadboard) LED <br>
* based on the value of potentiometer 0 <br>
* @param [in] none
* @param [out] none
*/
void led_brightness_thread()
{
    float brightness_pot_val;
    while(1)
    {
        brightness_pot_val = brightness_pot.read(); //read the current position of potentiometer 0, the one set used for brightness adjustment
        led_ld2_pwm.write(brightness_pot_val); //set the brightness of the LED based on the position of the potentiometer
        ThisThread::sleep_for(100); //pause to allow the brightness changes to be made
    }
}

/**
* Name: print_counter_thread <br>
* Description: This function prints a number that increases periodically <br>
* @param [in] none
* @param [out] none
*/
void print_counter_thread()
{
    uint8_t count = 0;
    
    // wait at start, so it doesn't immediately overwrite temperature
    ThisThread::sleep_for(PERIOD_PRINT_COUNT_MS / 2);
    
    while(1)
    {
        lcd_mutex.lock(); //"lock" the LCD so no other processes can write to it except this one
        lcd_printf("Count: %2d", count); //print the count to the LCD
        lcd_mutex.unlock(); //"unlock" the LCD so other processes can use it again
        ThisThread::sleep_for(PERIOD_PRINT_COUNT_MS); //pause to allow the text to be displayed on the screen
        
        // increase count, bounding by 2 characters
        count++;
        count = (count > 99) ? 0 : count;
    }
}

/**
* Name: print_counter_thread <br>
* Description: This function blinks our external (breadboard) LED at a frequency of 1Hz <br>
* @param [in] none
* @param [out] none
*/
void led_blink_thread()
{
    while(1)
    {
        led_external_on(); //turn the breadboard LED on
        ThisThread::sleep_for(PERIOD_LED_BLINK_MS / 2); //pause for 500ms
        led_external_off(); //turn the breadboard LED off
        ThisThread::sleep_for(PERIOD_LED_BLINK_MS / 2); //pause for 500ms
    }
}

/**
* Name: main <br>
* @param [in] none
* @param [out] none
*/
int main()
{
    temp_sensor_init(); //initialize the temperature sensor on the I2C bus
    
    lcd_init(); //initialize the LCD
    lcd_splash(); //print the LCD splash screen 
    
    Thread thread0(osPriorityNormal, OS_STACK_SIZE, NULL); //create thread 0
    thread0.start(print_temp_thread); //assign print_temp_thread to thread 0, then start the thread
    Thread thread1(osPriorityNormal, OS_STACK_SIZE, NULL); //create thread 1
    thread1.start(led_brightness_thread); //assign led_brightness_thread to thread 1, then start the thread
    Thread thread2(osPriorityNormal, OS_STACK_SIZE, NULL); //create thread 2
    thread2.start(print_counter_thread); //assign print_counter_thread to thread 2, then start the thread
    Thread thread3(osPriorityNormal, OS_STACK_SIZE, NULL); //create thread 3
    thread3.start(led_blink_thread); //assign led_blink_thread to thread 3, then start the thread
    
    while(1)
    {
        ThisThread::yield(); //make main yield to other threads
    }
}

/**
\file main.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

Main file of Project 2 Module 1

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "dhry.h"

///enable the onboard LED as a digital ouput
DigitalOut myled(LED1);

///set up the serial connection used with the terminal
Serial pc(SERIAL_TX, SERIAL_RX);

/**
* Name: main <br>
* @param [in] none
* @param [out] none
*/
int main() {
    
    dhry_main(1000000); //complete 1 million Dhrystone calculations
    
    pc.printf("RTC example\r\n"); 
    set_time(1539639581); // Set RTC time to 16 December 2013 10:05:23 UTC
    pc.printf("Date and time are set.\r\n");

    while(1) {

        time_t seconds = time(NULL); //obtain the current time in seconds
        
        pc.printf("Time as a basic string = %s\r\n", ctime(&seconds)); //print the time (set above) as a simple string

        myled = !myled; //cycle the LED

        wait(1); //wait one second
    }
}

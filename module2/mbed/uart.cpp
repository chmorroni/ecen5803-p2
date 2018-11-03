/**
\file uart.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

UART setup and printing functions for use in Project 2 Module 2 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "pin_mapping.h"
#include "uart.h"

///UART baud rate set to 115200
#define UART_BAUD (115200)

///set up the serial connection used with the terminal
Serial pc(USB_TX_PIN, USB_RX_PIN);

/**
* Name: uart_init <br>
* Description: this function initializes the uart serial connection at the defined baud rate <br>
* @param [in] none
* @param [out] none
*/
void uart_init()
{
    pc.baud(UART_BAUD);
}

/**
* Name: uart_init <br>
* Description: this function initializes the uart serial connection at the defined baud rate <br>
* @param [in] const char * format, ...
* @param [out] none
*/
void uart_printf(const char * format, ... )
{
    va_list args; //object that contains any extra arguments passed into the function after "format"
    va_start(args, format); //initialize the object, put the additional arguments in the object
    pc.vprintf(format, args); //print the string containing the uart message (format) and the associated values (found in args)
    va_end(args); //end the object
}
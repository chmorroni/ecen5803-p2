/**
\file uart.h

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

Function prototypes for the UART in Project 2 Module 2 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#ifndef __UART_H__
#define __UART_H__

void uart_init();
void uart_printf(const char * format, ... );

#endif
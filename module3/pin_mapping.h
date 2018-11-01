/**
\file pin_mapping.h

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>
Project 2 Module 3 <br>

Header file used for mapping STM NUCLEO F401RE pins to their physical components
*/

#ifndef __PIN_MAPPING_H__
#define __PIN_MAPPING_H__

/* Buttons */

///Button 0 attached to pin PA_10
#define BUTTON0_PIN (PA_10)
///Button 1 attached to pin PB_3
#define BUTTON1_PIN (PB_3)
///Button 2 attached to pin PB_5
#define BUTTON2_PIN (PB_5)
///Button 3 attached to pin PB_4
#define BUTTON3_PIN (PB_4)

/* LEDs */

///using on-board LED "LED1"
#define LED_LD2_PIN (LED1)
///external LED attached to pin PA_9
#define LED_EXTERNAL_PIN (PA_9)

/* Speaker */

///our piezoelectric speaker is attached to pin PB_10
#define SPEAKER_PIN (PB_10)

/* Potentiometers */

///Pin 0 of the potentiometer is attached to pin PA_0
#define POT0_PIN (PA_0)
///Pin 1 of the potentiometer is attached to pin PA_1
#define POT1_PIN (PA_1)

/* LCD */

///LCD "RS" pin is attached to pin PA_6
#define LCD_RS (PA_6)
///LCD "E" pin is attached to pin PA_7
#define LCD_E (PA_7)
///LCD data-bus pin 7 is attached to pin PA_4
#define LCD_DB7 (PA_4)
///LCD data-bus pin 6 is attached to pin PB_0
#define LCD_DB6 (PB_0)
///LCD data-bus pin 5 is attached to pin PC_1
#define LCD_DB5 (PC_1)
///LCD data-bus pin 4 is attached to pin PC_0
#define LCD_DB4 (PC_0)

/* I2C Temperature Sensor */

///I2C SCL line is attached to pin PB_8
#define TEMP_SCL_PIN (PB_8)
///I2C SDA line is attached to pin PB_9
#define TEMP_SDA_PIN (PB_9)

/* USB UART */

///Serial communication TX line is attached to pin PA_2
#define USB_TX_PIN (PA_2)
///Serial communication RX line is attached to pin PA_3
#define USB_RX_PIN (PA_3)

#endif
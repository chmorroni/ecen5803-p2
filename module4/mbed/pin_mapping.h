/**
\file pin_mapping.h

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>
Project 2 Module 4 <br>

Header file used for mapping STM NUCLEO F401RE pins to their physical components
*/

#ifndef __PIN_MAPPING_H__
#define __PIN_MAPPING_H__

/* Buttons */
#define BUTTON0_PIN (PA_10)
#define BUTTON1_PIN (PB_3)
#define BUTTON2_PIN (PB_5)
#define BUTTON3_PIN (PB_4)

/* LEDs */
#define LED_LD2_PIN (LED1)
#define LED_EXTERNAL_PIN (PA_9)

/* Speaker */
#define SPEAKER_PIN (PB_10)

/* Potentiometers */
#define POT0_PIN (PA_0)
#define POT1_PIN (PA_1)

/* LCD */
#define LCD_RS (PA_6)
#define LCD_E (PA_7)
#define LCD_DB7 (PA_4)
#define LCD_DB6 (PB_0)
#define LCD_DB5 (PC_1)
#define LCD_DB4 (PC_0)

/* I2C Temperature Sensor */
#define TEMP_SCL_PIN (PB_8)
#define TEMP_SDA_PIN (PB_9)

/* USB UART */
#define USB_TX_PIN (PA_2)
#define USB_RX_PIN (PA_3)

/* Waveforms */
#define WAVE_IN_PIN (PC_0)
#define WAVE_OUT_PIN (PC_9)

/* Debugging pins */
#define DBG_INTERRUPT_PIN (PC_8)
#define DBG_FFT_PIN (PC_6)

#endif

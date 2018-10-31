/**
\file main.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

Main file of Project 2 Module 2

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "lcd.h"
#include "led.h"
#include "temperature.h"
#include "uart.h"


#define PERIOD_PRINT_TEMP_S (1)

///maximum threshold for the potentiometer we are using
#define THRESHOLD (0.01f)

#define PWM_SET_INTERVAL_S (0.01f)
#define POT_PRINT_INTERVAL_S (1.0f)
///minimum period value possible when changing the period of the output waveform
#define MIN_PERIOD_US (125)
///maximum period value possible when changing the period of the output waveform
#define MAX_PERIOD_US (3125)

///flag to signal when to print the temperature
bool print_temp_flag = false;

///Ticker object used to print the temperature periodically
Ticker timer_print_temp;
///interrupt whenever button 0 is pressed
InterruptIn button0(BUTTON0_PIN);
///interrupt whenever button 1 is pressed
InterruptIn button1(BUTTON1_PIN);
///interrupt whenever button 2 is pressed
InterruptIn button2(BUTTON2_PIN);
///interrupt whenever button 3 is pressed
InterruptIn button3(BUTTON3_PIN);
///PWM output for the piezoelectric speaker
PwmOut speaker(SPEAKER_PIN);
///Declare potentiometer 0 (used to alter frequency) as an analog input
AnalogIn freq_pot(POT0_PIN);
///Declare potentiometer 1 (used to alter amplitude) as an analog input
AnalogIn amp_pot(POT1_PIN);

Ticker timer_pwm_set;
Ticker timer_pot_print;

/**
* Name: button0_isr <br>
* Description: this interrupt service routine turns the external (breadboard) led off <br>
* This ISR is run whenever button 0 is pressed
* @param [in] none
* @param [out] none
*/
void button0_isr() { led_external_off(); }

/**
* Name: button1_isr <br>
* Description: this interrupt service routine turns the external (breadboard) led on <br>
* This ISR is run whenever button 1 is pressed
* @param [in] none
* @param [out] none
*/
void button1_isr() { led_external_on(); }

/**
* Name: button2_isr <br>
* Description: this interrupt service routine turns the on-board led off <br>
* This ISR is run whenever button 2 is pressed
* @param [in] none
* @param [out] none
*/
void button2_isr() { led_ld2_off(); }

/**
* Name: button3_isr <br>
* Description: this interrupt service routine turns the on-board led on <br>
* This ISR is run whenever button 3 is pressed
* @param [in] none
* @param [out] none
*/
void button3_isr() { led_ld2_on(); }

/**
* Name: print_temp_isr <br>
* Description: this interrupt service routine prints the temperature to the LCD <br>
* This ISR is run whenever its timer overflows
* @param [in] none
* @param [out] none
*/
void print_temp_isr() { print_temp_flag = true; }

/**
* Name: calc_period_us <br>
* Description: this function calculates the period for the PWM signal based on the position of the potentiometer <br>
* @param [in] float pot_val
* @param [out] uint32_t
*/
uint32_t calc_period_us(float pot_val)
{
    uint32_t period = (MAX_PERIOD_US - MIN_PERIOD_US) * pot_val + MIN_PERIOD_US;
    period = period > MAX_PERIOD_US ? MAX_PERIOD_US : period;
    period = period < MIN_PERIOD_US ? MIN_PERIOD_US : period;
    return period;
}

/**
* Name: calc_period_us <br>
* Description: this function sets the pwm signal based on the positions of the two potentiometers <br>
* @param [in] none
* @param [out] none
*/
void pwm_set()
{
    static float freq_pot_val_last = 0, amp_pot_val_last = 0;
    
    float freq_pot_val = freq_pot.read();
    float amp_pot_val = amp_pot.read();
    
    if( abs(freq_pot_val - freq_pot_val_last) > THRESHOLD )
    {
        float period = calc_period_us(freq_pot_val);
        speaker.period_us(period);
        freq_pot_val_last = freq_pot_val;
    }
        
    if( abs(amp_pot_val - amp_pot_val_last) > THRESHOLD )
    {
        speaker.write(amp_pot_val / 4);
        amp_pot_val_last = amp_pot_val;
    }
}

/**
* Name: pot_print <br>
* Description: this function prints the current value of each potentiometer to the terminal <br>
* @param [in] none
* @param [out] none
*/
void pot_print()
{
    float freq_pot_val = freq_pot.read();
    float amp_pot_val = amp_pot.read();
    uart_printf("Frequency pot: %.6f, Amplitude pot: %.6f\r\n", freq_pot_val, amp_pot_val);
}

/**
* Name: print_temp <br>
* Description: this function prints the current temperature reading to the terminal <br>
* @param [in] none
* @param [out] none
*/
void print_temp()
{
    float temp = temp_measure();
    uart_printf("Current temperature: %5.2f deg C\r\n", temp);
    lcd_printf("Temp: %5.2f%cC", temp, 0xDF);
}

/**
* Name: main <br>
* @param [in] none
* @param [out] none
*/
int main()
{   
    uart_init();
    uart_printf("Hello mbed!\r\n");
    
    temp_sensor_init();
    
    lcd_init();
    lcd_splash();
    
    timer_print_temp.attach(print_temp_isr, PERIOD_PRINT_TEMP_S);
    button0.rise(button0_isr);
    button1.rise(button1_isr);
    button2.rise(button2_isr);
    button3.rise(button3_isr);
    timer_pwm_set.attach(pwm_set, PWM_SET_INTERVAL_S);
    timer_pot_print.attach(pot_print, POT_PRINT_INTERVAL_S);
    
    while(1)
    {
        if(print_temp_flag == true)
        {
            print_temp_flag = false;
            print_temp();
        }
        __wfi();
    }
}

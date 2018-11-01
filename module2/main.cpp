/**
\file main.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

Main file of Project 2 Module 2 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "lcd.h"
#include "led.h"
#include "temperature.h"
#include "uart.h"

///one second delay for use when printing the temperature to the screen
#define PERIOD_PRINT_TEMP_S (1)

///the smallest acceptable value change for the potentiometers we are using
#define THRESHOLD (0.01f)

///interval used to adjust the PWM output waveform every 10ms
#define PWM_SET_INTERVAL_S (0.01f)

///delay used to print a potentiometer value to the UART terminal every second
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

///Ticker object used to periodically run the function that sets the PWM signal
Ticker timer_pwm_set;

///Ticker object used to periodically print the potentiometer value to the terminal
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
    
    float freq_pot_val = freq_pot.read(); //read the value of the potentiometer used to adjust frequency
    float amp_pot_val = amp_pot.read(); //read the value of the potentiometer used to adjust amplitude
    
    if( abs(freq_pot_val - freq_pot_val_last) > THRESHOLD ) //if the current potentiometer value, minus its previous state, is greater than our preset threshold (see defines)
    {
        float period = calc_period_us(freq_pot_val); //calculate a new period based on the new potentiometer setting
        speaker.period_us(period); //apply the new period to the output waveform
        freq_pot_val_last = freq_pot_val; //save the current value for use as the previous value in the next calculation
    }
        
    if( abs(amp_pot_val - amp_pot_val_last) > THRESHOLD ) //if the current potentiometer value, minus its previous state, is greater than our preset threshold (see defines)
    {
        speaker.write(amp_pot_val / 4); //apply the new amplitude to the output waveform
        amp_pot_val_last = amp_pot_val; //save the current value for use as the previous value in the next calculation
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
    float freq_pot_val = freq_pot.read(); //read the current value of the potentiometer used for frequency
    float amp_pot_val = amp_pot.read(); //read the current value of the potentiometer used for amplitude
    uart_printf("Frequency pot: %.6f, Amplitude pot: %.6f\r\n", freq_pot_val, amp_pot_val); //print these values to the terminal
}

/**
* Name: print_temp <br>
* Description: this function prints the current temperature reading to the terminal <br>
* @param [in] none
* @param [out] none
*/
void print_temp()
{
    float temp = temp_measure(); //take a temperature measurement using the sensor
    uart_printf("Current temperature: %5.2f deg C\r\n", temp); //print the reading to the terminal
    lcd_printf("Temp: %5.2f%cC", temp, 0xDF); //print the reading to the LCD
}

/**
* Name: main <br>
* @param [in] none
* @param [out] none
*/
int main()
{   
    uart_init(); //initialize the UART
    uart_printf("Hello mbed!\r\n"); //print welcome message to the UART
    
    temp_sensor_init(); //initialize the temperature sensor
    
    lcd_init(); //initialize the LCD
    lcd_splash(); //print the LCD splash screen
    
    timer_print_temp.attach(print_temp_isr, PERIOD_PRINT_TEMP_S); //print the temperature reading to the LCD every second
    button0.rise(button0_isr); //run the ISR associated with button 0 (above) when the button is released
    button1.rise(button1_isr); //run the ISR associated with button 1 (above) when the button is released
    button2.rise(button2_isr); //run the ISR associated with button 2 (above) when the button is released
    button3.rise(button3_isr); //run the ISR associated with button 3 (above) when the button is released
    timer_pwm_set.attach(pwm_set, PWM_SET_INTERVAL_S); //check the positions of the potentiometers every 10ms and apply any resulting changes to the PWM signal
    timer_pot_print.attach(pot_print, POT_PRINT_INTERVAL_S); //print the positions of the potentiometers to the UART terminal every second
    
    while(1)
    {
        if(print_temp_flag == true) //if the flag requesting a temperature to be printed is raised
        {
            print_temp_flag = false; //lower the flag
            print_temp(); //print the temperature to the LCD
        }
        __wfi(); //wait for interrupt (sleep when not in use)
    }
}

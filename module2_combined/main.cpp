
#include "mbed.h"
#include "lcd.h"
#include "led.h"
#include "temperature.h"
#include "uart.h"

#define PERIOD_PRINT_TEMP_S (1)

#define THRESHOLD (0.01f)
#define PWM_SET_INTERVAL_S (0.01f)
#define POT_PRINT_INTERVAL_S (1.0f)
#define MIN_PERIOD_US (125)
#define MAX_PERIOD_US (3125)

bool print_temp_flag = false;

Ticker timer_print_temp;
InterruptIn button0(BUTTON0_PIN);
InterruptIn button1(BUTTON1_PIN);
InterruptIn button2(BUTTON2_PIN);
InterruptIn button3(BUTTON3_PIN);
PwmOut speaker(SPEAKER_PIN);
AnalogIn freq_pot(POT0_PIN);
AnalogIn amp_pot(POT1_PIN);
Ticker timer_pwm_set;
Ticker timer_pot_print;

void button0_isr() { led_external_off(); }
void button1_isr() { led_external_on(); }
void button2_isr() { led_ld2_off(); }
void button3_isr() { led_ld2_on(); }
void print_temp_isr() { print_temp_flag = true; }

uint32_t calc_period_us(float pot_val)
{
    uint32_t period = (MAX_PERIOD_US - MIN_PERIOD_US) * pot_val + MIN_PERIOD_US;
    period = period > MAX_PERIOD_US ? MAX_PERIOD_US : period;
    period = period < MIN_PERIOD_US ? MIN_PERIOD_US : period;
    return period;
}

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

void pot_print()
{
    float freq_pot_val = freq_pot.read();
    float amp_pot_val = amp_pot.read();
    uart_printf("Frequency pot: %.6f, Amplitude pot: %.6f\r\n", freq_pot_val, amp_pot_val);
}

void print_temp()
{
    float temp = temp_measure();
    uart_printf("Current temperature: %5.2f deg C\r\n", temp);
    lcd_printf("Temp: %5.2f%cC", temp, 0xDF);
}

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

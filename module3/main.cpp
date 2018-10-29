
#include "mbed.h"
#include "rtos.h"
#include "lcd.h"
#include "led.h"
#include "temperature.h"
#include "uart.h"

#include "pin_mapping.h"

#define PERIOD_PRINT_TEMP_MS (2000)
#define PERIOD_PRINT_COUNT_MS (2000)
#define PERIOD_LED_BLINK_MS (1000)

AnalogIn brightness_pot(POT0_PIN);
PwmOut led_ld2_pwm(LED_LD2_PIN);
Mutex lcd_mutex;

void print_temp_thread()
{
    float temp;
    while(1)
    {
        temp = temp_measure();
        lcd_mutex.lock();
        lcd_printf("Temp: %5.2f%cC", temp, 0xDF);
        lcd_mutex.unlock();
        ThisThread::sleep_for(PERIOD_PRINT_TEMP_MS);
    }
}

void led_brightness_thread()
{
    float brightness_pot_val;
    while(1)
    {
        brightness_pot_val = brightness_pot.read();
        led_ld2_pwm.write(brightness_pot_val);
        ThisThread::sleep_for(100);
    }
}

void print_counter_thread()
{
    uint8_t count = 0;
    
    // wait at start, so it doesn't immediately overwrite temperature
    ThisThread::sleep_for(PERIOD_PRINT_COUNT_MS / 2);
    
    while(1)
    {
        lcd_mutex.lock();
        lcd_printf("Count: %2d", count);
        lcd_mutex.unlock();
        ThisThread::sleep_for(PERIOD_PRINT_COUNT_MS);
        
        // increase count, bounding by 2 characters
        count++;
        count = (count > 99) ? 0 : count;
    }
}

void led_blink_thread()
{
    while(1)
    {
        led_external_on();
        ThisThread::sleep_for(PERIOD_LED_BLINK_MS / 2);
        led_external_off();
        ThisThread::sleep_for(PERIOD_LED_BLINK_MS / 2);
    }
}

int main()
{
    temp_sensor_init();
    
    lcd_init();
    lcd_splash();
    
    Thread thread0(osPriorityNormal, OS_STACK_SIZE, NULL);
    thread0.start(print_temp_thread);
    Thread thread1(osPriorityNormal, OS_STACK_SIZE, NULL);
    thread1.start(led_brightness_thread);
    Thread thread2(osPriorityNormal, OS_STACK_SIZE, NULL);
    thread2.start(print_counter_thread);
    Thread thread3(osPriorityNormal, OS_STACK_SIZE, NULL);
    thread3.start(led_blink_thread);
    
    while(1)
    {
        ThisThread::yield();
    }
}

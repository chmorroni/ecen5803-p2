/**
\file main.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

Main file of Project 2 Module 4 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "arm_math.h"
#include "adc.h"
#include "dma.h"
#include "pin_mapping.h"
#include "uart.h"
#include "waveform.h"

#define SAMPLE_PERIOD_US (52.08f)
#define SAMPLE_FREQ_HZ (1000000.0f / SAMPLE_PERIOD_US)
#define FFT_SIZE (4096)
#define FFT_BIN_SIZE_HZ (23.4375f)
#define MAG_THRESHOLD (100.0f)

#define DAC_FREQ_HZ (20000)
#define DAC_PERIOD_US (50)

#define SYSTICK_INTERRUPT_CYCLES (437)

#define ADC_BUCKET_SIZE (0.00080566406f)

PwmOut led_ld2_pwm(LED_LD2_PIN);
PwmOut wave_out(WAVE_OUT_PIN);
DigitalOut dbg_interrupt(DBG_INTERRUPT_PIN);
DigitalOut dbg_fft(DBG_FFT_PIN);
Ticker dac_timer;

uint16_t wave_buffer_0[FFT_SIZE];
bool wave_buffer_0_ready = false;
uint16_t wave_buffer_1[FFT_SIZE];
bool wave_buffer_1_ready = false;

/**
* Name: adc_read <br>
* Description: This function triggers an ADC read <br>
* @param [in] none
* @param [out] none
*/
void adc_read()
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

/**
* Name: dac_write <br>
* Description: This function writes a value from a lookup table to PWM <br>
* @param [in] none
* @param [out] none
*/
void dac_write()
{
    static uint32_t idx = 0;
//    static uint8_t mirrored = false;
//    static int8_t direction = 1;
    
    float val = waveform[idx];
//    val = (mirrored == true) ? 3.3f - val : val;
    wave_out.write(val / 3.3f);
    
//    if(direction == 1 && idx == 12000) direction = -1;
//    else if(direction == -1 && idx == 0) direction = 1;
//    idx += direction;
    if(idx == WAVEFORM_LEN) idx = 0;
    else idx++;
}

/**
* Name: SysTick_Handler <br>
* Description: This function triggers an ADC write when the SysTick interrupt fires <br>
* @param [in] none
* @param [out] none
*/
extern "C" void SysTick_Handler()
{
    dbg_interrupt = true;
    adc_read();
    dbg_interrupt = false;
}

/**
* Name: systick_init <br>
* Description: This function initializes the SysTick to interrupt at 192kHz <br>
* @param [in] none
* @param [out] none
*/
void systick_init()
{
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = SYSTICK_INTERRUPT_CYCLES;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/**
* Name: main <br>
* Description: This function initializes the system and calculates the FFT of samples from DMA <br>
* @param [in] none
* @param [out] none
*/
int main()
{
    bool fft_go = false;
    uint16_t * adc_buffer;
    float fft_samples[FFT_SIZE];
    float fft_freqs[FFT_SIZE];
    arm_rfft_fast_instance_f32 fft;
    float largest, largest_idx, major_freq;
    
    dma_init_double_buffered(wave_buffer_0, wave_buffer_1, FFT_SIZE);
    adc_init();
    uart_init();
    systick_init();
    
    uart_printf("Hello World!\r\n");
    
    arm_rfft_fast_init_f32(&fft, FFT_SIZE);
    
    led_ld2_pwm.write(0.5f);
    led_ld2_pwm.period(0.00099601593f);
    
    while(1)
    {
        // trigger FFT calculation
        if(wave_buffer_0_ready == true)
        {
            wave_buffer_0_ready = false;
            adc_buffer = wave_buffer_0;
            fft_go = true;
        }
        else if(wave_buffer_1_ready == true)
        {
            wave_buffer_1_ready = false;
            adc_buffer = wave_buffer_1;
            fft_go = true;
        }
        
        // perform FFT calculations
        if(fft_go == true)
        {
            fft_go = false;
            dbg_fft = true;
            
            // convert adc samples into floats
            for(uint32_t i = 0; i < FFT_SIZE; i++)
                fft_samples[i] = adc_buffer[i] * ADC_BUCKET_SIZE;
            
            arm_rfft_fast_f32(&fft, fft_samples, fft_freqs, 0);
            
            // find largest frequency component
            largest = 0;
            largest_idx = 0;
            for(uint32_t i = 1; i < FFT_SIZE / 2; i++)
            {
                if(fft_freqs[i] > largest)
                {
                    largest = fft_freqs[i];
                    largest_idx = i;
                }
            }
            major_freq = largest > MAG_THRESHOLD ? largest_idx * FFT_BIN_SIZE_HZ : 0;
            
            uart_printf("%5.2f: %5.2f\r\n", major_freq, largest);
            led_ld2_pwm.write(major_freq / 10000);

            dbg_fft = false;
        }
    }
}

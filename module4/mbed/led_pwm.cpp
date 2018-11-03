/**
\file led_pwm.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

PWM functions for Project 2 Module 4 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "led_pwm.h"

/**
* Name: pwm_led_init <br>
* Description: This function initializes Timer 2 to PWM the LD2 LED <br>
* @param [in] none
* @param [out] none
*/
void pwm_led_init()
{
    // enable the clock to the timer
    __HAL_RCC_TIM2_CLK_ENABLE();

    // initialize PA_5 to PWM mode
    GPIOA->MODER |= GPIO_MODER_MODER5_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER5_Msk | GPIO_MODER_MODER5_1;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5_Msk | GPIO_AFRL_AFSEL5_0;

    // initialize timer for PWM mode
    TIM2->CR1 = 0;
    TIM2->CR2 = 0;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->CCMR1 = (TIM_CCMR1_OC1M_Msk & (0x6 << TIM_CCMR1_OC1M_Pos)) | TIM_CCMR1_OC1PE;
    TIM2->CCER = TIM_CCER_CC1E;
    TIM2->ARR = 0xFFFF;
    TIM2->CCR1 = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
* Name: pwm_led_write <br>
* Description: This function sets the duty cycle of the LED PWM <br>
* @param [in] val The desired duty cycle [0..1]
* @param [out] none
*/
void pwm_led_write(float val)
{
    TIM2->CCR1 = 0xFFFF * val;
}
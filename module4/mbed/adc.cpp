/**
\file adc.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

ADC functions for Project 2 Module 4 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>

Examples found here: https://visualgdb.com/tutorials/arm/stm32/adc/
*/

#include "mbed.h"
#include "adc.h"

/**
* Name: adc_init <br>
* Description: This function initializes the ADC to convert in non-continuous mode and trigger the DMA <br>
* @param [in] none
* @param [out] none
*/
void adc_init()
{    
    GPIO_InitTypeDef gpioInit;
 
    __GPIOC_CLK_ENABLE();
    __ADC1_CLK_ENABLE();
    ADC1->CR2 |= ADC_CR2_ADON;
 
    gpioInit.Pin = GPIO_PIN_0;
    gpioInit.Mode = GPIO_MODE_ANALOG;
    gpioInit.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &gpioInit);
 
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
 
    ADC_ChannelConfTypeDef adcChannel;
    
    ADC_HandleTypeDef hadc;
    hadc.Instance = ADC1;
    
    hadc.Init.ClockPrescaler = ADC_SAMPLETIME_3CYCLES;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.ScanConvMode = DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.NbrOfDiscConversion = 0;
    hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.NbrOfConversion = 1;
    hadc.Init.DMAContinuousRequests = ENABLE;
    hadc.Init.EOCSelection = DISABLE;
    
    ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS;
    
    HAL_ADC_Init(&hadc);
    
    adcChannel.Channel = ADC_CHANNEL_10;
    adcChannel.Rank = 1;
    adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    adcChannel.Offset = 0;
 
    HAL_ADC_ConfigChannel(&hadc, &adcChannel);
}

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

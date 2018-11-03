/**
\file adc.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

DMA functions for Project 2 Module 4 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>

Examples found here: http://www.lucadavidian.com/2017/11/17/stm32f4-using-the-dma-controller-with-adc/
*/

#include "mbed.h"
#include "dma.h"

extern bool wave_buffer_0_ready;
extern bool wave_buffer_1_ready;

/**
* Name: dma_init_double_buffered <br>
* Description: This function initializes DMA to write to dual buffers triggered from the ADC <br>
* @param [in] buf_0 The first buffer to write
* @param [in] buf_1 The second buffer to write
* @param [in] buf_size The size of each buffer
* @param [out] none
*/
void dma_init_double_buffered(void * buf_0, void * buf_1, uint32_t buf_size)
{
    /* enable DMA 2 clock */
    __HAL_RCC_DMA2_CLK_ENABLE();
 
    /* configure DMA2 */
    DMA2_Stream0->CR |= 0U << DMA_SxCR_ACK_Pos;       // ADC1 on DMA2 stream 0 channel 0
    DMA2_Stream0->CR |= 0x01 << DMA_SxCR_PSIZE_Pos;   // ADC data register is 16 bit (half word)
    DMA2_Stream0->CR |= 0x01 << DMA_SxCR_MSIZE_Pos;   // memory size is is 16 bit (half word)
    DMA2_Stream0->CR |= DMA_SxCR_DBM;                 // double buffer mode
    DMA2_Stream0->CR |= DMA_SxCR_MINC;                // memory increment (MSIZE = 16 bit)
    DMA2_Stream0->CR |= DMA_SxCR_CIRC;                // circular mode
    DMA2_Stream0->CR |= 0x00 << DMA_SxCR_DIR_Pos;     // peripheral to memory
    DMA2_Stream0->CR |= 0x03 << DMA_SxCR_PL_Pos;      // priority level very high
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;          // peripheral base address
    DMA2_Stream0->M0AR = (uint32_t)buf_0;             // memory base address 0
    DMA2_Stream0->M1AR = (uint32_t)buf_1;             // memory base address 1
    DMA2_Stream0->NDTR = buf_size;                    // number of data
    DMA2_Stream0->FCR &= ~DMA_SxFCR_DMDIS;            // direct mode (FIFO disabled)
    
    /* enable DMA2 stream 0 interrupt */
    DMA2_Stream0->CR |= DMA_SxCR_TCIE;                // enable transfer complete interrupt
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);                // enable stream 0 IRQ in NVIC
 
    /* enable DMA2 stream 0 */
    DMA2_Stream0->CR |= DMA_SxCR_EN;
}

/**
* Name: DMA2_Stream0_IRQHandler <br>
* Description: This function triggers an FFT calculation in main <br>
* @param [in] none
* @param [out] none
*/
extern "C" void DMA2_Stream0_IRQHandler(void)
{
    /* transmission complete interrupt */
    if (DMA2->LISR & DMA_LISR_TCIF0 && DMA2_Stream0->CR & DMA_SxCR_TCIE)
    {
        DMA2->LIFCR = DMA_LIFCR_CTCIF0;  // acknowledge interrupt
        
        if ((DMA2_Stream0->CR & DMA_SxCR_CT) == 0) // current target buffer 0 (read buffer 1)
            wave_buffer_1_ready = true;
        else                                       // current target buffer 1 (read buffer 0)
            wave_buffer_0_ready = true;
    }
}

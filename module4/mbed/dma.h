/**
\file adc.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

DMA function prototypes for Project 2 Module 4 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>

Examples found here: http://www.lucadavidian.com/2017/11/17/stm32f4-using-the-dma-controller-with-adc/
*/

#ifndef __DMA_H__
#define __DMA_H__

void dma_init_double_buffered(void * buf_0, void * buf_1, uint32_t buf_size);

#endif

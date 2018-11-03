/**
\file led_pwm.h

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

PWM functions for Project 2 Module 4 <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#ifndef __LED_PWM_H__
#define __LED_PWM_H__

void pwm_led_init();
void pwm_led_write(float val);

#endif

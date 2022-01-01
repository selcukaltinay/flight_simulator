/*
 * joystick.c
 *
 *  Created on: Dec 17, 2020
 *      Author: selcukaltinay
 */
#include "stm32f429xx.h"
#include "joystick.h"

/**
  * @brief
  * @param  GPIO_PIN_x
  * @param  GPIO_PIN_x
  * @retval
  */

void joystickInit(uint16_t pinRx,uint16_t pinRy, uint8_t ADCx_joystick )
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= ((0x3 << pinRx*2) | (0x3 << pinRy*2));
	RCC->APB2ENR |= (1 << ADCx_joystick);


}

void getJoystickValue()
{


}


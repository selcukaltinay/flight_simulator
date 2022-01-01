/*
 * joystick.h
 *
 *  Created on: Dec 17, 2020
 *      Author: selcukaltinay
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_


#define ADC1_joystick 8
#define ADC2_joystick 9
#define ADC3_joystick 10


void joystickInit(uint16_t pinRx,uint16_t pinRy);

void getJoystickValue();


#endif /* INC_JOYSTICK_H_ */

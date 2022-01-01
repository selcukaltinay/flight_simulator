/*
 * BMP180.h
 *
 *  Created on: Dec 19, 2020
 *      Author: selcukaltinay
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "stm32f4xx_hal.h"

#define BMP180_READ_ADD 0xEF
#define BMP180_WRITE_ADD 0xEE
#define COUNT_OF_CAL_VAL 22

extern I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef *pi2c;

int16_t AC1; // 8 bit msb 8 bit lsb
int16_t AC2;
int16_t AC3;
int16_t AC4;
int16_t AC5;
int16_t AC6;
int16_t B1;
int16_t B2;
int16_t MB;
int16_t MC;
int16_t MD;

uint8_t dataRead[2];

int16_t uncTemp; //Uncompansated Temperature
float comTemp;
int16_t uncPressure;
float comPressure;

int32_t tX1, tX2, tB5;

int32_t pX1, pX2, pX3, pB3, pB4, pB6, pB7;


void BMP180_Init();
void BMP180_GetCalibrationValue();
float BMP180_GetTemperature();
uint16_t BMP180_GetPressure();




#endif /* INC_BMP180_H_ */

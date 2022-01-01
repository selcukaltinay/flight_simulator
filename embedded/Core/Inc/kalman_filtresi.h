/*
 * kalman_filtresi.h
 *
 *  Created on: Dec 17, 2020
 *      Author: selcu
 */

#ifndef INC_KALMAN_FILTRESI_H_
#define INC_KALMAN_FILTRESI_H_


typedef struct{

	float kalYeni;
	float kalEski;
	float kovYeni;
	float kovEski;
	float kalKazanci;
	float kalHesaplanmis;
}Kalman_TypeDef;



float kalman_filtresi(Kalman_TypeDef , float, float, float);


#endif /* INC_KALMAN_FILTRESI_H_ */

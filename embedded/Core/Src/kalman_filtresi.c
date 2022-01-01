/*
 * kalman_filtresi.c
 *
 *  Created on: Dec 17, 2020
 *      Author: selcu
 */
#include "kalman_filtresi.h"

float kalman_filtresi(Kalman_TypeDef kalman_t, float input, float kalmanQ, float kalmanR)
{
	kalman_t.kalYeni = kalman_t.kalEski;
	kalman_t.kovYeni = kalman_t.kovEski + kalmanQ;
	kalman_t.kalKazanci = kalman_t.kovYeni / (kalman_t.kovYeni + kalmanR);
	kalman_t.kalHesaplanmis = kalman_t.kalYeni + (kalman_t.kalKazanci * (input - kalman_t.kalYeni));

	kalman_t.kovYeni = (1 - kalman_t.kalKazanci)*kalman_t.kovEski;
	kalman_t.kovEski=kalman_t.kalYeni;

	kalman_t.kalEski = kalman_t.kalYeni;


	return kalman_t.kalHesaplanmis;
}


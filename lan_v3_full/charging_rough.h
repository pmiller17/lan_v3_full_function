/*
 * charging_rough.h
 *
 * Created: 4/30/2014 8:59:23 PM
 *  Author: mini me
 */ 


#ifndef CHARGING_ROUGH_H_
#define CHARGING_ROUGH_H_

#include "lan.h"

#define OVERVOLTAGE 219 // about 6.7 V
#define BULK_CURRENT 246 // about 750 mA
#define TAPER_CURRENT 40 // about 125 mA

void charge_battery(void);
void initialize_charging_mode(void);

typedef enum
{
	CONSTANT_CURRENT,
	CONSTANT_VOLTAGE,
	TRICKLE_CHARGE
	} charging_mode_t;


#endif /* CHARGING_ROUGH_H_ */
/*
 * adc.c
 *
 * Created: 4/9/2014 5:39:29 PM
 *  Author: mini me
 */ 

#include "adc.h"
#include "lan.h"

unsigned int adc_read_iled()
{
	volatile unsigned int dummy;
	CFG_ADC_ILED_GAIN_20;
	ADC_TRIGGER_MEASUREMENT;
	dummy = ADCH;
	ADC_TRIGGER_MEASUREMENT;
	dummy = ADCH;
	return dummy;
}

unsigned int adc_read_vbatt(void)
{	
	volatile unsigned int dummy;
	CFG_ADC_VBATT_GAIN_1;
	ADC_TRIGGER_MEASUREMENT;
	dummy = ADCH;
	ADC_TRIGGER_MEASUREMENT;
	dummy = ADCH;
	return dummy;
}

unsigned int adc_read_ibatt(void)
{
	volatile unsigned int dummy;
	CFG_ADC_IBATT_GAIN_32;
	ADC_TRIGGER_MEASUREMENT;
	dummy = ADCH;
	ADC_TRIGGER_MEASUREMENT;
	dummy = ADCH;
	return dummy;
}


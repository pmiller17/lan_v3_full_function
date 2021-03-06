/*
 * charging_rough.c
 *
 * Created: 4/30/2014 8:59:05 PM
 *  Author: mini me
 */ 

#include "lan.h"
#include "adc.h"
#include "lighting.h"
#include "charging_rough.h"

charging_mode_t charging_mode;
unsigned long int overvoltage_thresh_count;
unsigned long int enough_bulk_current_counter = 0;
unsigned int bulk_charge_reached = FALSE;
unsigned long int trickle_charge = 0;
unsigned long int not_enough_volts = 0;
unsigned int go_back_bulk = 0;
unsigned int current_offset = 0;
volatile int battery_voltage;
volatile int battery_current;


void initialize_charging_mode(void)
{
	overvoltage_thresh_count = 0;
	enough_bulk_current_counter = 0;
	bulk_charge_reached = FALSE;
	trickle_charge = 0;
	not_enough_volts = 0;
	go_back_bulk = FALSE;
	current_offset = 0;
	battery_current = 0;
	battery_voltage = 0;
	OCR1B = 0;
	OCR1B = 1;
	charging_mode = CONSTANT_CURRENT;
	CFG_BUTTON_OFF;
	TC0_OVF_INT_ENABLE;		
	ADC_ENABLE;
//	ADC_ISR_ENABLE;
	FPWM_CLR_COMP_MATCH;
	TURN_ON_PWM_CLK;
}
#if 0
void charge_battery(void)
{
	
	switch(charging_mode)
	{

		case CONSTANT_CURRENT:
			
			battery_current = adc_read_ibatt();

			current_offset = battery_current/320;
			
			if(battery_current < 242)
			{
				OCR1B++;
			}
			else if(battery_current > 245)
			{
				OCR1B--;
			}
		
			if(battery_current >= 242)
			{			
				enough_bulk_current_counter++;
				if(enough_bulk_current_counter > 2000)
				{
					bulk_charge_reached = TRUE;
				}
			}
		
			if((battery_current < 234) & (battery_current > 0))
			{
				enough_bulk_current_counter = 0;
			}
			
			battery_voltage = adc_read_vbatt();			
			if(bulk_charge_reached == TRUE)
			{
				if(battery_voltage - current_offset > 220)
				{
					overvoltage_thresh_count++;
				}
			
				if(battery_voltage - current_offset < 220)
				{
					overvoltage_thresh_count = 0;
				}
			}
			
			if(battery_voltage - current_offset > 240)
			{
				OCR1B -= 3;
			}
			
			if(overvoltage_thresh_count >= 2000)
			{
				charging_mode = CONSTANT_VOLTAGE;
			}
			
		break;

		case CONSTANT_VOLTAGE:
			
			battery_voltage = adc_read_vbatt();
			battery_current = adc_read_ibatt();
			current_offset = battery_current/320;
			
			if(battery_voltage - current_offset > 220)
			{
				OCR1B -= 1;
			}
		
			else if(battery_voltage - current_offset < 220)
			{
				OCR1B += 3;
			}
		
			if(battery_voltage - current_offset < 208)
			{
				not_enough_volts++;
				if(not_enough_volts >= 1000)
				{
					go_back_bulk = TRUE;
				}
			}
		
			if(battery_voltage - current_offset > 216)
			{
				not_enough_volts = 0;
			}
		
			if(battery_voltage > 240)
			{
				OCR1B = 160;
			}
		
			if(battery_voltage < 170)
			{
				OCR1B = 130;
			}
		
			if(go_back_bulk)
			{
				charging_mode = CONSTANT_CURRENT;
				go_back_bulk = FALSE;
			}	
		
			if(battery_current < 30)
			{
				trickle_charge++;
			}
		
			if(battery_current > 50)
			{
				trickle_charge = 0;
			}
		
			if(trickle_charge >= 2000)
			{
				charging_mode = TRICKLE_CHARGE;
				TC0_DONE_CHARGING_RATE;
			}
		
		break;
		
		case TRICKLE_CHARGE:
			
			battery_voltage = adc_read_vbatt();
			battery_current = adc_read_ibatt();
			current_offset = battery_current/320;
			
			if(battery_voltage - current_offset > 220)
			{
				OCR1B -= 1;
			}
		
			else if(battery_voltage - current_offset < 220)
			{
				OCR1B += 3;		
			}
		
		break;
	}
}
#endif

void charge_battery(void)
{
	battery_current = adc_read_ibatt();
	battery_voltage = adc_read_vbatt();
	
	if(battery_current < BULK_CURRENT && battery_voltage < OVERVOLTAGE)
	{
		OCR1B++;
	}
	
	else if(battery_current > BULK_CURRENT || battery_voltage > OVERVOLTAGE)
	{
		OCR1B--;
	}
	
	if(battery_voltage > (OVERVOLTAGE - 3) && battery_current < TAPER_CURRENT)
	{
		trickle_charge++;
	}
	else trickle_charge = 0;
	
	if(trickle_charge > 50)
	{
		TC0_DONE_CHARGING_RATE;
	}
	
	
	
}
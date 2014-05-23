/*
 * lighting.c
 * 
 * Created: 5/8/2014 9:36:51 PM
 *  Author: Paul Miller
 *	All functions to do with LED control and flickering
 */ 

#include "lan.h"
#include "adc.h"
#include "lighting.h"

#define DIM_CURRENT 35	// about 35 mA, 35/255 max reading
#define BRIGHT_CURRENT 102 // about 100 mA, 100/255 max reading
#define FLICKER_ON_TIME 1000 // just a guess
#define FLICKER_OFF_TIME 1000 // just a guess
#define FLICKER_TARGET_CURRENT 20 // just for blink -- current value not critical
#define DIM_ENERGY 134.218 // number of seconds between interrupts 
#define BRIGHT_ENERGY 335.545 
// multiply above by 100/40 to account for how much effective runtime is added on bright mode

#define BATTERY_CUTOFF 178 // 5.6V

lighting_mode_t lighting_mode;

void cycle_led_mode(void)
{
	switch(lighting_mode)
	{
		case OFF:
			lighting_mode = DIM;
			TC0_OVF_INT_ENABLE;
			TC0_RUNTIME_COUNTER_RATE;
			FPWM_CLR_COMP_MATCH;
			LED_ENABLE;
			break;
			
		case DIM:
			lighting_mode = BRIGHT;
			break;
			
		case BRIGHT:
			lighting_mode = OFF;
			TC0_OVF_INT_DISABLE;
			TC0_STOP;
			FPWM_NORMAL_PORT_OP;
			OCR1B = 0;
			LED_DISABLE;
			break;
	}
}

void initialize_lighting_mode(void) // move this one to lan_v3_newtest.c
{
	LED_ENABLE;
	BUTTON_PCI_ENABLE;
	TC0_STOP;
	lighting_mode = OFF;
}

unsigned int is_battery_too_low(void)
{
	unsigned int battery_level;
	unsigned static int too_low_count;
	battery_level =	adc_read_vbatt();
	
	if(battery_level <= BATTERY_CUTOFF)
	{
		too_low_count++;
	}
	else if(battery_level > BATTERY_CUTOFF)
	{
		too_low_count = 0;
	}
	
	if(too_low_count > 5)
		return TRUE;
	
	else return FALSE;
	
}
void flicker_led(void) // may want to have input be number of times
{
	int i; //for wasting time in a loop	
	for(i = 0; i < FLICKER_ON_TIME; i++)
		led_control_current(FLICKER_TARGET_CURRENT);
	
	OCR1B = 0;
	
	for(i = 0; i < FLICKER_OFF_TIME; i++)
		;
	
}

void led_control_current(uint8_t target_current)
{
	
	uint8_t led_current;
	led_current = adc_read_iled();
	if(led_current < target_current)
		OCR1B++;
	else if(led_current > target_current)
		OCR1B--;
	
}

void run_lighting_mode(void)
{
	
	switch(lighting_mode)
	{
		case OFF:
			
			OCR1B = 0;

		//add the <avr/sleep.h> code here
		break;
		//probably could use a pointer but ohwell
		case DIM:
			//add wakeup code here
			led_control_current(DIM_CURRENT);
		break;
		
		case BRIGHT:
			led_control_current(BRIGHT_CURRENT);
		break;
	}

}

float calculate_lantern_usage(void)
{
	
	// just return a scale amount
	float battery_increment = 0;
		
	if(lighting_mode == DIM)
	battery_increment = DIM_ENERGY;
		
	else if(lighting_mode == BRIGHT)
	battery_increment = BRIGHT_ENERGY;
		
	return battery_increment;
}

void led_charging_indicate(void)
{
	/* in order to minimize
		impact on charging algorithm
		use a constant OCR1B for a flicker instead of current control
		
		store the OCR1B value from charging,
		
		change OCR1B to ~5 or 10
		enable LED;
		wait a few cycles
		disable LED;
		change OCR1B back to stored value;
		
	*/
	volatile unsigned int timewaster = 0;
	volatile unsigned int charging_pulse_width;
	charging_pulse_width = OCR1B;
	OCR1B = 10;
	LED_ENABLE;
	while(timewaster < 3000)
		timewaster++;
	LED_DISABLE;
	OCR1B = charging_pulse_width;
}
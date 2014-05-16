/*
	Paul Miller
	5/16/2014
	
	lan_v3 with full switch case for complete lantern operation


*/

#include <avr/sleep.h>

#include "lan.h"
#include "adc.h"
#include "debounce.h"
#include "charging_rough.h"
#include "lighting.h"

#define TRUE 1
#define FALSE 0

#define BATTERY_USAGE_LIMIT

volatile lantern_mode_t lantern_mode, previous_mode;
volatile task_t task;
int button_pressed = 0;
int jack_plugged_in = 0;
int battery_usage = 0;

int main(void)
{
	setup();
	for(;;)
	{
		loop();
	}
}

void setup(void)
{
	cli();

    int i;
	for(i = 0; i <= 7; i++)
	{
		CLRBIT(PCMSK0,i);
		CLRBIT(PCMSK1,i);
	}
  
    /*  Enable the PLL.  */
    ENABLE_PLL;
    while(PLL_IS_NOT_LOCKED)
    {
        // Waiting for PLOCK bit to be set
    }
    ENABLE_PCK;

    /*  Use a prescaler of 16 to set the system clock at 500 kHz (1 MHz?).
    */
    SET_SYS_CLK;

    /*  IO Configurations
    */
	CFG_IO_BUTTON;		
	CFG_IO_LED_ENABLE;
	CFG_IO_JACK;	
    CFG_IO_PWM;			
    
    /*  Clock 1 (PWM clock) Configurations
        Use PLL to set clock 1 at 64 MHz, asynchronous from the system clock.
        Use a prescaler of 4 and a TOP value of 256 to have FPWM frequency at 62.5 kHz.
    */
    CFG_PWM;
    FPWM_NORMAL_PORT_OP;
    TURN_OFF_PWM_CLK;
	OCR1C = 255;
    OCR1B = 0;
		
    CFG_ADC;

    ADC_ENABLE;
//	ADC_ISR_ENABLE;
    
    LED_ENABLE;

	PCI_ENABLE;
	BUTTON_PCI_ENABLE;
	JACK_PCI_ENABLE;
	
	FPWM_CLR_COMP_MATCH;
	TURN_ON_PWM_CLK;
#if 0	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
#endif
	
	battery_current = 0;
	battery_voltage = 0;
	pwm_value = 0;
    sei();
}

void loop(void)
{
	switch(lantern_mode)
	{
		case LIGHTING:
			if(task.debounce_button)
			{	
				button_pressed = debounce_button();
				task.debounce = FALSE;
			}
			
			if(button_pressed)
			{
				cycle_led_mode();
				button_pressed = FALSE;
			}
			
			if(task.update_runtime)
			{
				battery_usage = calculate_lantern_usage();
				task.update_runtime = FALSE;
			}
			
			if(battery_usage > BATTERY_USAGE_LIMIT)
				lantern_mode = NEEDS_CHARGE;
			
			/*need function to say "light LED"?*/
			run_lighting_mode();	//will sleep if "OFF" and control light otherwise
			
		break;

			
		case CHARGING:
			charge_battery();

		break;
		
		case NEEDS_CHARGE:
			if(task.debounce_button)
			{
				button_pressed = debounce_button();
			}
			
			if(button_pressed)
			{
				flicker_led();
				button_pressed = FALSE;
			}

		break;

		case SAFE_OFF:
			
			if(task.debounce_jack)
			{
				jack_plugged_in = debounce_jack();
				task.debounce_jack = FALSE;
			}	
			if(jack_plugged_in)
			{
				lantern_mode = CHARGING;
			}
			else
			{
				lantern_mode = previous_mode;
				set_up_noncharging(); 
				/*	re-enable stuff, set light to OFF?
					button & jack PCI enables.
				*/
			}
			
		break;

	}

}


ISR(TIMER0_OVF_vect)
{
	task.update_runtime = TRUE;
}

ISR(PCINT_vect)
{
	if(JACK_PLUGGED_IN_NOW)
	{
		LED_DISABLE;
		/*other safety disables*/
		previous_mode = lantern_mode;
		lantern_mode = SAFE_OFF;
		task.debounce_jack = TRUE; 
	}
	
	else if(BUTTON_PRESSED_NOW)
	{
		task.debounce_button = TRUE;
	}
	
	else if(JACK_PLUGGED_IN_NOW == FALSE)
	{
		previous_mode = lantern_mode;
		lantern_mode = SAFE_OFF;
	}
}
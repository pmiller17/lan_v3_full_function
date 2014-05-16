/*
debounce.c
contains function that goes through the debounce states if the lantern task says "debounce"
debounce the button, debounce the jack
 */ 

#include "debounce.h"
#include "lan.h"


int debounce_jack(void)
{
	int jack_plugged_in;
	volatile int debounce_counter = 0;
	
	while(debounce_counter < JACK_DELAY_TIME)
	{
		debounce_counter += 1;
	}
	
	if (JACK_PLUGGED_IN_NOW)
	{
		jack_plugged_in = TRUE;
		debounce_counter = 0;
		
		while(debounce_counter < JACK_IGNORE_TIME)
		{
			debounce_counter += 1;
		}
		
	}
	
	else
	{
		jack_plugged_in = FALSE;
	}
	

	debounce_counter = 0;
	BUTTON_PCI_ENABLE;
	JACK_PCI_ENABLE;	//define!
	return jack_plugged_in;
}

int debounce_button(void)
{
	int button_pressed = FALSE;
	volatile int debounce_counter = 0;
	
	while(debounce_counter < BUTTON_DELAY_TIME)
	{
		debounce_counter += 1;
	}
		
	if (BUTTON_PRESSED_NOW)
	{
		button_pressed = TRUE;
		debounce_counter = 0;
			
		while(debounce_counter < BUTTON_IGNORE_TIME)
		{
			debounce_counter += 1;
		}
			
	}
		
	else
	{
		button_pressed = FALSE;
	}
		

	debounce_counter = 0;
	BUTTON_PCI_ENABLE;
	return button_pressed;
	
}

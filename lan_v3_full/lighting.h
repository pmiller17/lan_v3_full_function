/*
 * lighting.h
 *
 * Created: 5/8/2014 9:47:21 PM
 *  Author: mini me
 */ 


#ifndef LIGHTING_H_
#define LIGHTING_H_



/*------------------------------------------
	LIGHTING-RELATED-MACROS
-------------------------------------------*/


#define CFG_RUNTIME TIMER do				\ 
{											\
	CLRBIT(TCCR0A,ICEN0);					\
	SETBIT(TCCR0A,TCW0);					\
	

} while (0);


//-----FUNCTION PROTOTYPES-------------------

void cycle_led_mode(void);
void initialize_lighting_mode(void);
void flicker_led(void); // may want to have input be number of times
void led_control_current(uint8_t);
void run_lighting_mode(void);
unsigned int calculate_lantern_usage(void);

typedef enum
{
	OFF,
	DIM,
	BRIGHT,
	} lighting_mode_t;

#endif /* LIGHTING_H_ */
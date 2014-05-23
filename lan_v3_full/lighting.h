/*
 * lighting.h
 *
 * Created: 5/8/2014 9:47:21 PM
 *  Author: mini me
 */ 

#include "lan.h"

#ifndef LIGHTING_H_
#define LIGHTING_H_



/*------------------------------------------
	LIGHTING-RELATED-MACROS
-------------------------------------------*/

// when counting how long lantern's been running, interrupt every 133 seconds
// io_clk / 1024
#define CFG_TC0_RUNTIME do					\
{											\
	CLRBIT(TCCR0A,ICEN0);					\
	SETBIT(TCCR0A,TCW0);					\
	SETBIT(TCCR0B,CS02);					\
	CLRBIT(TCCR0B,CS01);					\
	SETBIT(TCCR0B,CS00);					\
} while (0)

//when done charging, blink every 8.3 seconds (io_clk/64)
#define TC0_DONE_CHARGING_RATE do			\
{											\
	CLRBIT(TCCR0B,CS02);					\
	SETBIT(TCCR0B,CS01);					\
	SETBIT(TCCR0B,CS00);					\
} while (0)

//when charging, blink light every 33 seconds (io_clk/256)
#define TC0_INDICATE_CHARGE_RATE do			\
{											\
	SETBIT(TCCR0B,CS02);					\
	CLRBIT(TCCR0B,CS01);					\
	CLRBIT(TCCR0B,CS00);					\
} while (0)

// when running light, every 134 seconds interrupt to update (io_clk/1024)
#define TC0_RUNTIME_COUNTER_RATE do			\
{											\
	SETBIT(TCCR0B,CS02);					\
	CLRBIT(TCCR0B,CS01);					\
	SETBIT(TCCR0B,CS00);					\
} while (0)

#define TC0_STOP do							\
{											\
	SETBIT(TCCR0B,CS02);					\
	CLRBIT(TCCR0B,CS01);					\
	SETBIT(TCCR0B,CS00);					\
} while (0)

#define TC0_OVF_INT_ENABLE SETBIT(TIMSK,TOIE0);
#define TC0_OVF_INT_DISABLE CLRBIT(TIMSK,TOIE0);

//-----FUNCTION PROTOTYPES-------------------

void cycle_led_mode(void);
void initialize_lighting_mode(void);
void flicker_led(void); // may want to have input be number of times
void led_control_current(uint8_t);
void run_lighting_mode(void);
float calculate_lantern_usage(void);
void led_charging_indicate(void);
unsigned int is_battery_too_low(void);

//clearing battery_usage should be done as part of changing modes in the switch-case

typedef enum
{
	OFF,
	DIM,
	BRIGHT,
	} lighting_mode_t;

#endif /* LIGHTING_H_ */
#ifndef LAN_H_
#define LAN_H_

#include <avr/io.h>
#include <avr/interrupt.h>


/*  Function Prototypes
*/
void setup(void);
void loop(void);

//=========================================================
// TRUE FALSE MACROS
//---------------------------------------------------------
#define FALSE 0
#define TRUE 1

//=========================================================
// BIT OP MACROS
//---------------------------------------------------------
#define SETBIT(ADDRESS,BIT) (ADDRESS |= (unsigned char)(1<<BIT))
#define CLRBIT(ADDRESS,BIT) (ADDRESS &= (unsigned char)~(1<<BIT))
#define	CHKBIT(ADDRESS,BIT) (ADDRESS & (unsigned char)(1<<BIT))
// modify so that it returns either 1 or 0 by having it reshift back to LSB?
//usage: if (CHKBIT(ADDRESS,BIT)) and NOT if(CHKBIT(ADDRESS,BIT)==1) until modified

//=========================================================
// PIN NAMES
//---------------------------------------------------------

#define BUTTON_PIN PB0
#define LED_ENABLE_PIN PB1
#define PWM_PIN	PB3
#define JACK_PIN PB4 

//=========================================================
// PLL MACROS
//---------------------------------------------------------
#define ENABLE_PLL SETBIT(PLLCSR,PLLE)
#define PLL_IS_NOT_LOCKED (CHKBIT(PLLCSR,PLOCK) == 0)
#define ENABLE_PCK SETBIT(PLLCSR,PCKE)

//=========================================================
// SYSTEM CLOCK SETTING MACRO
//---------------------------------------------------------
#define SET_SYS_CLK                     \
    do                                  \
    {                                   \
        CLKPR = (1<<CLKPCE);            \
        CLKPR = (1<<CLKPS2);            \
    }                                   \
    while (0)

//=========================================================
// CFG MACROS
//---------------------------------------------------------
#define CFG_IO_BUTTON                   \
    do                                  \
    {                                   \
        CLRBIT(DDRB,BUTTON_PIN);        \
        SETBIT(PORTB,BUTTON_PIN);       \
    }                                   \
    while (0)

#define CFG_IO_JACK						\
	do									\
	{									\
		CLRBIT(DDRB,JACK_PIN);			\
		CLRBIT(PORTB,JACK_PIN);			\
	}									\
	while (0);
	
#define CFG_IO_LED_ENABLE               \
    do                                  \
    {                                   \
        SETBIT(DDRB,LED_ENABLE_PIN);    \
        CLRBIT(PORTB,LED_ENABLE_PIN);   \
    }                                   \
    while(0)



#define CFG_IO_PWM                      \
    do                                  \
    {                                   \
        SETBIT(DDRB,PWM_PIN);           \
        CLRBIT(PORTB,PWM_PIN);          \
    }                                   \
    while(0)

#define CFG_PWM                         \
    do                                  \
    {                                   \
        CLRBIT(TCCR1A,COM1B1);          \
        CLRBIT(TCCR1A,COM1B0);          \
        SETBIT(TCCR1A,PWM1B);           \
                                        \
        CLRBIT(TCCR1D,WGM11);           \
        CLRBIT(TCCR1D,WGM10);           \
    }                                   \
    while(0)

#define FPWM_NORMAL_PORT_OP             \
    do                                  \
    {                                   \
        CLRBIT(TCCR1A,COM1B1);          \
    }                                   \
    while(0)

#define FPWM_CLR_COMP_MATCH             \
    do                                  \
    {                                   \
        SETBIT(TCCR1A,COM1B1);          \
    }                                   \
    while(0)
    
#define TURN_ON_PWM_CLK                 \
    do                                  \
    {                                   \
        CLRBIT(TCCR1B,CS13);            \
        CLRBIT(TCCR1B,CS12);            \
        SETBIT(TCCR1B,CS11);            \
        SETBIT(TCCR1B,CS10);            \
    }                                   \
    while(0)

#define TURN_OFF_PWM_CLK                \
    do                                  \
    {                                   \
        CLRBIT(TCCR1B,CS13);            \
        CLRBIT(TCCR1B,CS12);            \
        CLRBIT(TCCR1B,CS11);            \
        CLRBIT(TCCR1B,CS10);            \
    }                                   \
    while(0)

#define LED_ENABLE                      \
    do                                  \
    {                                   \
        SETBIT(PORTB,LED_ENABLE_PIN);   \
    }                                   \
    while(0)

 #define LED_DISABLE                    \
    do                                  \
    {                                   \
        CLRBIT(PORTB,LED_ENABLE_PIN);   \
    }                                   \
    while(0)
    
#define CTC0 0
#define CFG_SLOW_PWM                    \
    do                                  \
    {                                   \
        CLRBIT(TCCR0A,ICEN0);           \
        CLRBIT(TCCR0A,TCW0);            \
        SETBIT(TCCR0A,CTC0);            \
    }                                   \
    while(0)

#define TURN_ON_SLOW_PWM_CLK            \
    do                                  \
    {                                   \
        CLRBIT(TCCR0B,CS02);            \
        SETBIT(TCCR0B,CS01);            \
        CLRBIT(TCCR0B,CS00);            \
    }                                   \
    while(0)

#define TURN_OFF_SLOW_PWM_CLK           \
    do                                  \
    {                                   \
        CLRBIT(TCCR0B,CS02);            \
        CLRBIT(TCCR0B,CS01);            \
        CLRBIT(TCCR0B,CS00);            \
    }                                   \
    while(0)
    
#define SLOW_PWM_ISR_ENABLE             \
    do                                  \
    {                                   \
        SETBIT(TIMSK,OCIE0A);           \
        SETBIT(TIMSK,OCIE0B);           \
    }                                   \
    while(0)

#define SLOW_PWM_ISR_DISABLE            \
    do                                  \
    {                                   \
        CLRBIT(TIMSK,OCIE0A);           \
        CLRBIT(TIMSK,OCIE0B);           \
    }                                   \
    while(0)
//=========================================================
// PIN CHANGE INTERRUPT MACROS
//---------------------------------------------------------

//PCI_ENABLE also configures the button pin!
//This is unclear and not very modular.  Just clear all the interrupts
//use other macros to set the interrupt enables.


#define PCI_ENABLE                      \
    do                                  \
    {                                   \
        SETBIT(GIMSK,PCIE1);            \
		SETBIT(GIMSK,PCIE0);			\
    }                                   \
    while(0)

#define BUTTON_PCI_ENABLE               \
    do                                  \
    {                                   \
        SETBIT(PCMSK1,PCINT8);          \
    }                                   \
    while(0)

#define BUTTON_PCI_DISABLE              \
    do                                  \
    {                                   \
        CLRBIT(PCMSK1,PCINT8);          \
    }                                   \
    while(0)


#define JACK_PCI_ENABLE					\
	do									\
	{									\
		SETBIT(PCMSK1,PCINT12);			\
	}									\
	while(0)

#define JACK_PCI_DISABLE				\
	do									\
	{									\
		CLRBIT(PCMSK1,PCINT12);			\
	}									\
	while(0)
//=========================================================
// MACRO FOR IF-STATEMENT ARGUMENT
//---------------------------------------------------------

#define BUTTON_PRESSED_NOW  (CHKBIT(PINB,BUTTON_PIN) == 0)
#define JACK_PLUGGED_IN_NOW (CHKBIT(PINB,JACK_PIN))

//=========================================================
// STRUCTURE DEFINITIONS
//---------------------------------------------------------


typedef enum
{
    LIGHTING,
    CHARGING,
	SAFE_OFF,
	NEEDS_CHARGE
} lantern_mode_t;

typedef struct
{
	uint8_t debounce_button : 1;
	uint8_t debounce_jack	: 1;
	uint8_t timer			: 1;
} task_t;

#endif


/*
 * adc.h
 *
 * Created: 4/9/2014 5:39:39 PM
 *  Author: mini me
 */ 


#ifndef ADC_H_
#define ADC_H_

unsigned int adc_read_iled(void);
unsigned int adc_read_vbatt(void);
unsigned int adc_read_ibatt(void);

//=========================================================
// ADC MACROS
//---------------------------------------------------------

//CFG_ADC
//clear REFS1 and set REFS0 in ADMUX to choose Vref = AREF PIN
//set ADLAR --> left adjust the conversion result
//keep for all ADCs
#define CFG_ADC                         \
do                                  \
{                                   \
	CLRBIT(ADMUX,REFS1);            \
	SETBIT(ADMUX,REFS0);            \
	SETBIT(ADMUX,ADLAR);            \
	CLRBIT(ADCSRA,ADATE);			\
}                                   \
while(0)

//CFG_ADC_VBATT_GAIN_1
//specifically for choosing the gain for Vbatt ADC
//This is diff ADC between ADC4-ADC3, or pins 13-14
//Table for selecting and controlling an ADC's gain,
//Differential vs single ended mode located on page 157
//MUX[5:0] bits in ADMUX and ADCSRB are configured

//For other ADC's to be configured, need their own
//according to schematic:
//
#define CFG_ADC_VBATT_GAIN_1        \
do                                  \
{                                   \
	CLRBIT(ADCSRB,MUX5);            \
	SETBIT(ADMUX,MUX4);             \
	CLRBIT(ADMUX,MUX3);             \
	CLRBIT(ADMUX,MUX2);             \
	SETBIT(ADMUX,MUX1);             \
	SETBIT(ADMUX,MUX0);             \
	CLRBIT(ADCSRB,GSEL);            \
}                                   \
while(0)

#define CFG_ADC_IBATT_GAIN_32       \
do                                  \
{                                   \
	SETBIT(ADCSRB,MUX5);            \
	CLRBIT(ADMUX,MUX4);             \
	CLRBIT(ADMUX,MUX3);             \
	CLRBIT(ADMUX,MUX2);             \
	SETBIT(ADMUX,MUX1);             \
	CLRBIT(ADMUX,MUX0);             \
	SETBIT(ADCSRB,GSEL);            \
}                                   \
while(0)

#define CFG_ADC_ILED_GAIN_20        \
do                                  \
{                                   \
	CLRBIT(ADCSRB,MUX5);            \
	CLRBIT(ADMUX,MUX4);             \
	SETBIT(ADMUX,MUX3);             \
	SETBIT(ADMUX,MUX2);             \
	SETBIT(ADMUX,MUX1);             \
	CLRBIT(ADMUX,MUX0);             \
	CLRBIT(ADCSRB,GSEL);            \
}                                   \
while(0)

#define ADC_ENABLE                  \
do                                  \
{                                   \
	SETBIT(ADCSRA,ADEN);            \
}                                   \
while(0)

#define ADC_DISABLE                 \
do                                  \
{                                   \
	CLRBIT(ADCSRA,ADEN);            \
}                                   \
while(0)

#define ADC_ISR_ENABLE              \
do                                  \
{                                   \
	SETBIT(ADCSRA,ADIE);            \
}                                   \
while(0)

#define ADC_ISR_DISABLE             \
do                                  \
{                                   \
	CLRBIT(ADCSRA,ADIE);            \
}                                   \
while(0)

#define ADC_TRIGGER_MEASUREMENT		\
do									\
{                                   \
	SETBIT(ADCSRA,ADSC);            \
	while(ADC_IN_PROGRESS);         \
}                                   \
while(0)

#define ADC_IN_PROGRESS (CHKBIT(ADCSRA,ADSC))

#endif /* ADC_H_ */
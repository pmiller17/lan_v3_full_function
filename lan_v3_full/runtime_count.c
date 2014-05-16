/* notes and code for figuring out how long you can run lantern
	eventually move to lighting.c

*/

/*

	Timer/Counter 0 - use in 16-bit mode
	
	Clock internally & use prescaler clk_io/1024
	
	for 500 kHz clock,
	
	488.28125 Hz, or 2.048 ms
	
	16 bits means 2^16 to overflow, so 65536 steps
	
	65536*0.002048 seconds = 134.217728 seconds between overflow
	or 2.2369621333 minutes
	
	100 hours / 2.237 mins = 2682 overflows
	
	just store in an unsigned int
	
	(re-calc the error due to cycles lost in the interrupt)
	
	increment-return.  say 10 cycles @ 500 kHz (conservative)
	20 us * 2, nah not important  53 ms.
	
	Mode 2: Normal mode 16-bit
	ICEN0 = 0
	TCW0 = 1
	CTC0 = X
	
	this makes TOP 0xFFFF, where TOV flag is set
	
	TCCR0A - Timer/Counter0 Control Register A
	bit 7 - TCW0: Timer/Counter0 Width - 1 means 16 bit
		0 means 8 bit
		
	bit 6 - ICEN0 - Input Capture Mode Enable
		1 means enabled
	
	bit 5, 4 are input capture things - noise and edge select
	bit 3 - ACIC0 - analog comparator input capture (0 == no connect)
	bit 2:1 reserved - always will read 0
	bit 0 - CTC0: Waveform Generation Mode
	
	TCCR0B - Timer/Counter Control Register B
	
	Bits 7:5 read only
	
	Bit 4 - TSML Timer/Counter Synchronization Mode - keeps prescaler 
		bit written, and reset signal asserted
		
	
	
	How it works:  Timer overflows, increment a runtime counter
	
	runtime counter can then be scaled by current rate to update a scaled counter
	
	2250 hours on sleep - WDT can wake up and increment
	
	*/
	

	
	if(battery_used > BATTERY_USAGE_LIMIT)
		lantern_mode = NEEDS_CHARGE;
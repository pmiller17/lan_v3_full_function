/*
debounce.h


Paul Miller
4/7/2014

*/ 



#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#define BUTTON_DELAY_TIME 8000		// 8 ms
#define BUTTON_IGNORE_TIME 7000		// 140 ms

#define JACK_DELAY_TIME 400			// 8 ms
#define JACK_IGNORE_TIME 1000		// 20 ms

// reset delay and ignore time to 400 and 7000, respectively

int debounce_button(void);
int debounce_jack(void);

// debounce state machine definitions

#endif /* DEBOUNCE_H_ */
// 
// 
// 

#include "PIN.h"

void PINClass::init()
{
	//for (int i = 0; i < 5; i++) {
	//	pinMode(RELAY_CH1[i], OUTPUT);
	//}

	//for (int i = 0; i < 5; i++) {
	//	pinMode(RELAY_CH2[i], OUTPUT);
	//}

	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);

	//init pin latch to high
	digitalWrite(latchPin, HIGH);

	pinMode(AMP_CH1, INPUT);
	pinMode(FREQ_RISING_CH1, INPUT);

	pinMode(AMP_CH2, INPUT);
	pinMode(FREQ_RISING_CH2, INPUT);

	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, HIGH);

	pinMode(DC_CH1, INPUT);
	pinMode(DC_CH2, INPUT);
}


PINClass PIN;


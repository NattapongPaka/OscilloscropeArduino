// PIN.h

#ifndef _PIN_h
#define _PIN_h
#include "Arduino.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class PINClass
{
	
 protected:


 public:
	void init();

	const int DC_CH1 = A2;
	const int DC_CH2 = A3;
	//const int RELAY_CH1[5] = { 0,1,2,3,4 };
	const int AMP_CH1 = A0;
	const int FREQ_RISING_CH1 = 2;

	//const int RELAY_CH2[5] = { 5,6,7,8,9 };
	const int AMP_CH2 = A1;
	const int FREQ_RISING_CH2 = 3;

	const int CLK = 13;      // DCLK
	const int MISO = 12;     // DOUT
	const int MOSI = 11;     // DIN
	const int CS = 10;       // CS

	const byte SPI_CH0 = 0x94;
	const byte SPI_CH1 = 0xD4;
	const byte SPI_CH2 = 0xA4;
	const byte SPI_CH3 = 0xE4;
	
	const int latchPin = 6;		// ST_CP
	const int clockPin = 7;		// SH_CP
	const int dataPin = 5;		// DS

	const int led_pin = 4;
};

extern PINClass PIN;

#endif


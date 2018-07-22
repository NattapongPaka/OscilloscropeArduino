// ADS7841.h

#ifndef _ADS7841_h
#define _ADS7841_h
#include "Arduino.h"
#include "SPI.h"


#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class ADS7841Class
{
 protected:


 public:
	void init();
	void sendData(byte command);
	int receiveData();
};

extern ADS7841Class ADS7841;

#endif


// 
// 
// 
#include "ADS7841.h"
#include "PIN.h"

int value[2];
//int SPI_BUAD_RATE = 15000000;

void ADS7841Class::init()
{
	pinMode(PIN.CS, OUTPUT);
	pinMode(PIN.MOSI, OUTPUT);
	pinMode(PIN.MISO, INPUT);
	pinMode(PIN.CLK, OUTPUT);

	digitalWrite(PIN.CS, HIGH);

	SPISettings mSet = SPISettings(3200000, MSBFIRST, SPI_MODE0);
	SPI.begin();
	SPI.beginTransaction(mSet);
}

void ADS7841Class::sendData(byte command)
{	
	digitalWrite(PIN.CS, LOW);
	//Send in command byte
	SPI.transfer(command);
	//Get two result bytes
	value[0] = SPI.transfer(0);
	value[1] = SPI.transfer(0);
	digitalWrite(PIN.CS, HIGH);
}

int ADS7841Class::receiveData()
{
	int resultAnalog = value[0];
	resultAnalog = resultAnalog << 8;
	resultAnalog = resultAnalog | value[1];
	resultAnalog = resultAnalog >> 3;
	return resultAnalog;
}


ADS7841Class ADS7841;


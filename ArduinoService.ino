#include "PIN.h"
#include "CMD.h"
#include "ADS7841.h"
#include "Thread.h"
#include "ThreadController.h"
#include "FreqPeriodCounter.h"

const int analogBufferMax = 100;
const unsigned long BAUD_RATE = 115200;

volatile int count;

String analogBuffer1;
String analogBuffer2;

unsigned int Freq1;
unsigned int Freq2;

const byte counterPin1 = PIN.FREQ_RISING_CH1;
const byte counterPin2 = PIN.FREQ_RISING_CH2;

FreqPeriodCounter counter1(counterPin1, millis);
FreqPeriodCounter counter2(counterPin2, millis);

Thread thread1 = Thread();
Thread thread2 = Thread();
ThreadController threadController = ThreadController();

/*Test Simulate*/
//int mode_ac = 13;
//int mode_dc = 12;
/*Test Simulate*/

void setup() {

	Serial.begin(BAUD_RATE);
	CMD.init();
	PIN.init();
	ADS7841.init();

	thread1.onRun(task1);
	thread1.setInterval(1);

	thread2.onRun(task2);
	thread2.setInterval(1);

	threadController.add(&thread1);
	threadController.add(&thread2);

	attachInterrupt(digitalPinToInterrupt(counterPin1), counterISR1, CHANGE);
	attachInterrupt(digitalPinToInterrupt(counterPin2), counterISR2, CHANGE);

	/*Test Simulate*/
	//pinMode(mode_ac, INPUT);
	//pinMode(mode_dc, INPUT);
	/*Test Simulate*/
}

void counterISR1()
{
	counter1.poll();
}

void counterISR2()
{
	counter2.poll();
}

void task1() {
	if (CMD.getRunning() == CMD_RUN) {
		if (count < analogBufferMax) {
			// Channel 1
			if (CMD.getChannel() == CMD_CH1) {
				if (CMD.getMode() == CMD_MODE_VOLT) {
					if (CMD.getType() == CMD_TYPE_AC) {
						serialAppendData(CMD_CH1, readAnalog(PIN.SPI_CH0));
						//Freq1 = readFreq(CMD_CH1);
					}
					if (CMD.getType() == CMD_TYPE_DC) {
						//serialAppendData(CMD_CH1, readAnalog(PIN.SPI_CH1));
						serialAppendData(CMD_CH1, readAnalogDC(PIN.DC_CH1));
					}
				}
				if (CMD.getMode() == CMD_MODE_AMP) {
					serialAppendData(CMD_CH1, readAmp(CMD_CH1));
				}
			}
			// Channel 2
			else if (CMD.getChannel() == CMD_CH2) {
				if (CMD.getMode() == CMD_MODE_VOLT) {
					if (CMD.getType() == CMD_TYPE_AC) {
						serialAppendData(CMD_CH2, readAnalog(PIN.SPI_CH2));
						//Freq2 = readFreq(CMD_CH2);
					}
					if (CMD.getType() == CMD_TYPE_DC) {
						//serialAppendData(CMD_CH2, readAnalog(PIN.SPI_CH3));
						serialAppendData(CMD_CH2, readAnalogDC(PIN.DC_CH2));
					}
				}
				if (CMD.getMode() == CMD_MODE_AMP) {
					serialAppendData(CMD_CH2, readAmp(CMD_CH2));
				}
			}
			// All Channel
			else if (CMD.getChannel() == CMD_ALL) {
				if (CMD.getMode() == CMD_MODE_VOLT) {
					if (CMD.getType() == CMD_TYPE_AC) {
						serialAppendData(CMD_CH1, readAnalog(PIN.SPI_CH0));
						//Freq1 = readFreq(CMD_CH1);
						serialAppendData(CMD_CH2, readAnalog(PIN.SPI_CH2));
						//Freq2 = readFreq(CMD_CH2);
					}
					if (CMD.getType() == CMD_TYPE_DC) {
						//serialAppendData(CMD_CH1, readAnalog(PIN.SPI_CH1));
						//serialAppendData(CMD_CH2, readAnalog(PIN.SPI_CH3));
						serialAppendData(CMD_CH1, readAnalogDC(PIN.DC_CH1));
						serialAppendData(CMD_CH2, readAnalogDC(PIN.DC_CH2));
					}
				}
				if (CMD.getMode() == CMD_MODE_AMP) {
					serialAppendData(CMD_CH1, readAmp(CMD_CH1));
					serialAppendData(CMD_CH2, readAmp(CMD_CH2));
				}
			}
			// Increment count
			count++;
		}
	}
	else {
		Serial.clearWriteError();
		Serial.flush();
		//CMD.setDefault();
		count = 0;
	}
}

void task2() {
	if (count == analogBufferMax) {
		int Ch = CMD.getChannel();
		int Type = CMD.getType();
		int Mode = CMD.getMode();

		switch (Ch)
		{
		case CMD_CH1:
			Serial.print(Ch);			// Channel	Data
			Serial.print(",");
			Serial.print(Type);			// Type	Data
			Serial.print(",");
			Serial.print(Mode);			// Mode	Data
			Serial.print(",");
			Serial.print(Freq1);		// Freq	Data
			Serial.print(",");
			Serial.print(CMD_START);	// Start Data
			Serial.print(",");
			Serial.print(analogBuffer1);// Analog Data
			Serial.println(CMD_END);	// End Data
			Serial.flush();				// Waiting to send data
			break;

		case CMD_CH2:
			Serial.print(Ch);			// Channel Data
			Serial.print(",");
			Serial.print(Type);			// Type	Data
			Serial.print(",");
			Serial.print(Mode);			// Mode Data
			Serial.print(",");
			Serial.print(Freq2);		// Freq	Data
			Serial.print(",");
			Serial.print(CMD_START);
			Serial.print(",");
			Serial.print(analogBuffer2);
			Serial.println(CMD_END);
			Serial.flush();
			break;

		case CMD_ALL:
			/****************CH1****************/
			Serial.print(CMD_CH1);		// Channel Data
			Serial.print(",");
			Serial.print(Type);			// Type	Data
			Serial.print(",");
			Serial.print(Mode);			// Mode Data
			Serial.print(",");
			Serial.print(Freq1);		// Freq	Data
			Serial.print(",");
			Serial.print(CMD_START);
			Serial.print(",");
			Serial.print(analogBuffer1);
			Serial.println(CMD_END);
			Serial.flush();
			/****************CH2****************/
			Serial.print(CMD_CH2);		// Channel Data
			Serial.print(",");
			Serial.print(Type);			// Type	Data
			Serial.print(",");
			Serial.print(Mode);			// Mode Data
			Serial.print(",");
			Serial.print(Freq2);		// Freq	Data
			Serial.print(",");
			Serial.print(CMD_START);
			Serial.print(",");
			Serial.print(analogBuffer2);
			Serial.println(CMD_END);
			Serial.flush();
			break;

		}
		analogBuffer1 = "";
		analogBuffer2 = "";
		count = 0;
	}
}

void loop() {

	threadController.run();

	/*Test Simulate*/
	//if (digitalRead(mode_ac) == LOW) {
	//	CMD.setChannel(CMD_CH1);
	//	CMD.setType(CMD_TYPE_DC);
	//	//CMD.setMode(CMD_MODE_AMP);
	//}

	//if (digitalRead(mode_dc) == LOW) {
	//	CMD.setChannel(CMD_CH2);
	//	CMD.setType(CMD_TYPE_DC);
	//	//CMD.setMode(CMD_MODE_AMP);
	//}
	/*Test Simulate*/

	/*
	Start Read Freq
	*/
	if (CMD.getRunning() == CMD_RUN) {
		if (CMD.getMode() == CMD_MODE_VOLT) {
			if (CMD.getType() == CMD_TYPE_AC) {
				if (CMD.getChannel() == CMD_ALL) {			
					if (counter1.ready())Freq1 = (counter1.elapsedTime > 50000) ? 0 : counter1.hertz(1);
					if (counter2.ready())Freq2 = (counter2.elapsedTime > 50000) ? 0 : counter2.hertz(1);
				}
				if (CMD.getChannel() == CMD_CH1 && counter1.ready()) {
					Freq1 = (counter1.elapsedTime > 50000) ? 0 : counter1.hertz(1);
					Freq2 = 0;
				}
				if (CMD.getChannel() == CMD_CH2 && counter2.ready()) {
					Freq2 = (counter2.elapsedTime > 50000) ? 0 : counter2.hertz(1);
					Freq1 = 0;
				}
			}
			else {
				Freq1 = 0;
				Freq2 = 0;
			}
		}
	}
	/*
	End Read Freq
	*/
}

int readAmp(int ch) {
	int amp = 0;
	if (ch == CMD_CH1) {
		amp = analogRead(PIN.AMP_CH1);
	}
	else if (ch == CMD_CH2) {
		amp = analogRead(PIN.AMP_CH2);
	}
	return amp;
}

//double readFreq(int ch) {
//	double freq = 0;
//	if (ch == CMD_CH1) {	
//		if (counter.ready()) {
//			//Freq1 = (float)1.0f / counter.period;
//			freq = counter.hertz(1);
//		}
//		//freq = (double)1.0 / counter.period;
//		//freq = (double)counter.hertz(1);
//		//freq = analogRead(PIN.FREQ_RISING_CH1);
//	}
//	else if (ch == CMD_CH2) {
//		freq = analogRead(PIN.FREQ_RISING_CH2);
//	}
//	return freq;
//}

int readAnalog(byte ch) {
	ADS7841.sendData(ch);
	return ADS7841.receiveData();
}

int readAnalogDC(int ch) {
	return analogRead(ch);
}

void serialAppendData(int ch, int value) {
	switch (ch)
	{
	case CMD_CH1:
		analogBuffer1.concat(value);
		analogBuffer1.concat(",");
		break;

	case CMD_CH2:
		analogBuffer2.concat(value);
		analogBuffer2.concat(",");
		break;
	}
}

void updateStatus() {
	int Ch = CMD.getChannel();
	int Type = CMD.getType();
	int Mode = CMD.getMode();
	String status = "";
	status.concat(Ch);
	status.concat(",");
	status.concat(Type);
	status.concat(",");
	status.concat(Mode);
	Serial.println(status);
	Serial.flush();
}

void serialEvent() {
	int c;
	Serial.clearWriteError();
	while (Serial.available())
	{
		c = Serial.parseInt();
		Serial.flush();
	}

	//Serial.println(c);
	switch (c)
	{
	case CMD_CH1:
		CMD.setChannel(CMD_CH1);
		break;

	case CMD_CH2:
		CMD.setChannel(CMD_CH2);
		break;

	case CMD_TYPE_AC:
		CMD.setType(CMD_TYPE_AC);
		break;

	case CMD_TYPE_DC:
		CMD.setType(CMD_TYPE_DC);
		break;

	case CMD_MODE_VOLT:
		CMD.setMode(CMD_MODE_VOLT);
		break;

	case CMD_MODE_AMP:
		CMD.setMode(CMD_MODE_AMP);
		break;

	case CMD_RUN:
		CMD.setRunning(CMD_RUN);
		break;

	case CMD_STOP:
		CMD.setRunning(CMD_STOP);
		break;

	case CMD_ALL:
		CMD.setChannel(CMD_ALL);
		break;

	case CMD_AUTO:
		CMD.setDefault();
		break;

	case CMD_INIT:
		CMD.offRelay(CMD_CH1);
		CMD.offRelay(CMD_CH2);
		break;
	}
	updateStatus();
}

/*
Loop Backup
//// RUN
//if (CMD.getRunning() == CMD_RUN) {
//	if (count < analogBufferMax) {
//		// Channel 1
//		if (CMD.getChannel() == CMD_CH1) {
//			if (CMD.getMode() == CMD_MODE_VOLT) {
//				if (CMD.getType() == CMD_TYPE_AC) {
//					analogSerialBuffer1[count] = readAnalog(CH0);
//				}
//				if (CMD.getType() == CMD_TYPE_DC) {
//					analogSerialBuffer1[count] = readAnalog(CH0);
//				}
//			}
//			if (CMD.getMode() == CMD_MODE_AMP) {

//			}
//		}
//		// Channel 2
//		else if (CMD.getChannel() == CMD_CH2) {
//			if (CMD.getMode() == CMD_MODE_VOLT) {
//				if (CMD.getType() == CMD_TYPE_AC) {
//					analogSerialBuffer2[count] = readAnalog(CH1);
//				}
//				if (CMD.getType() == CMD_TYPE_DC) {

//				}
//			}
//			if (CMD.getMode() == CMD_MODE_AMP) {

//			}
//		}
//		// All Channel
//		else if (CMD.getChannel() == CMD_ALL) {
//			analogSerialBuffer1[count] = readAnalog(CH0);
//			analogSerialBuffer2[count] = readAnalog(CH1);
//		}

//		//Serial.println(analogSerialBuffer1[count]);
//		//analogSerialBuffer1[count] = readAnalog(CH0);
//		serialAppendData(analogSerialBuffer1[count]);
//		count++;
//	}
//	//// Send data if count = analogBufferMax
//	//else if(count == analogBufferMax) {
//	//	count = 0;
//	//	Serial.print(CMD_START);
//	//	Serial.print(",");
//	//	Serial.print(analogSerial);
//	//	Serial.println(CMD_END);
//	//	Serial.flush();
//	//	analogSerial = "";
//	//	//serialSendData(analogSerialBuffer1);
//	//}
//}
//// STOP
//else {
//	Serial.clearWriteError();
//	Serial.flush();
//	CMD.setDefault();
//	count = 0;
//}
*/
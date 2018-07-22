// 
// 
// 
#include "CMD.h"
#include "PIN.h"

const double SCAL_VOLT = 6.06;
const boolean isConnect = true;
const int BUFF_CAP = 512;
const int TRIGGER = 10;
const int MESURE = 11;
const int GEN = 21;
const int IOIO = 22;

void CMDClass::init() {
	//Set Defult
	CMD.Channel = CMD_CH1;
	CMD.Type = CMD_TYPE_DC;
	CMD.Mode = CMD_MODE_VOLT;
	CMD.Running = CMD_STOP;
}



void DataOut_Ch1(unsigned int data_Ch1) {
	digitalWrite(PIN.latchPin, LOW);
	shiftOut(PIN.dataPin, PIN.clockPin, MSBFIRST, B00000000);
	shiftOut(PIN.dataPin, PIN.clockPin, MSBFIRST, data_Ch1);
	digitalWrite(PIN.latchPin, HIGH);
}

void DataOut_Ch2(unsigned int data_Ch2) {
	digitalWrite(PIN.latchPin, LOW);
	shiftOut(PIN.dataPin, PIN.clockPin, MSBFIRST, data_Ch2);
	shiftOut(PIN.dataPin, PIN.clockPin, MSBFIRST, B00000000);
	digitalWrite(PIN.latchPin, HIGH);
}

void DataOut_All(unsigned int data_Ch1, int data_Ch2) {
	digitalWrite(PIN.latchPin, LOW);
	shiftOut(PIN.dataPin, PIN.clockPin, MSBFIRST, data_Ch2);
	shiftOut(PIN.dataPin, PIN.clockPin, MSBFIRST, data_Ch1);
	digitalWrite(PIN.latchPin, HIGH);
}

void CMDClass::writeRelayCH1(int r1) {
	DataOut_Ch1(r1);
	delay(10);
}

void CMDClass::writeRelayCH2(int r2) {
	DataOut_Ch2(r2);
	delay(10);
}

void CMDClass::writeRelayCHALL(int rl, int r2) {
	DataOut_All(rl, r2);
	delay(10);
}

void CMDClass::offRelay(int ch) {
	if (ch == CMD_CH1) {
		DataOut_Ch1(RL_OFF);
		delay(10);
	}
	else if (ch == CMD_CH2) {
		DataOut_Ch2(RL_OFF);
		delay(10);
	}
}

void CMDClass::setDefault() {
	CMD.Channel = CMD_CH1;
	CMD.Type = CMD_TYPE_DC;
	CMD.Mode = CMD_MODE_VOLT;
	CMD.Running = CMD_STOP;
}

void CMDClass::setChannel(int ch) {
	Channel = ch;
	CMD.status[0] = ch;
	updateStatus();
}

void CMDClass::setType(int type) {
	Type = type;
	CMD.status[1] = type;
	updateStatus();
}

void CMDClass::setMode(int mode) {
	Mode = mode;
	CMD.status[2] = mode;
	updateStatus();
}

void CMDClass::setRunning(int running) {
	Running = running;
	CMD.status[3] = running;
	updateStatus();
}

int CMDClass::getChannel() {
	return Channel;
}

int CMDClass::getType() {
	return Type;
}

int CMDClass::getMode() {
	return Mode;
}

int CMDClass::getRunning() {
	return Running;
}

void CMDClass::updateStatus() {
	// Channel 1
	if (CMD.Channel == CMD_CH1) {
		offRelay(CMD_CH2);
		if (CMD.Mode == CMD_MODE_VOLT) {
			if (CMD.Type == CMD_TYPE_DC) {
				writeRelayCH1(CMD.RL1_DC);
			}
			else if (CMD.Type == CMD_TYPE_AC) {
				writeRelayCH1(CMD.RL1_AC);
			}
		}
		else if (CMD.Mode == CMD_MODE_AMP) {
			writeRelayCH1(CMD.RL1_AMP);
		}
	}
	// Channel 2
	else if (CMD.Channel == CMD_CH2) {
		offRelay(CMD_CH1);
		if (CMD.Mode == CMD_MODE_VOLT) {
			if (CMD.Type == CMD_TYPE_DC) {
				writeRelayCH2(CMD.RL2_DC);
			}
			else if (CMD.Type == CMD_TYPE_AC) {
				writeRelayCH2(CMD.RL2_AC);
			}
		}
		else if (CMD.Mode == CMD_MODE_AMP) {
			writeRelayCH2(CMD.RL2_AMP);
		}
	}
	// All Channel
	else if (CMD.Channel == CMD_ALL) {
		if (CMD.Mode == CMD_MODE_VOLT) {
			if (CMD.Type == CMD_TYPE_DC) {				
				writeRelayCHALL(CMD.RL1_DC, CMD.RL2_DC);
			}
			else if (CMD.Type == CMD_TYPE_AC) {
				writeRelayCHALL(CMD.RL1_AC, CMD.RL2_AC);
			}
		}
		else if (CMD.Mode == CMD_MODE_AMP) {
			writeRelayCHALL(CMD.RL1_AMP, CMD.RL2_AMP);
		}
	}
}

CMDClass CMD;


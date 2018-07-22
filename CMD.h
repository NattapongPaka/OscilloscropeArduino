// CMD.h

#ifndef _CMD_h
#define _CMD_h
#include "Arduino.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class CMDClass
{
// Command id control data
#define CMD_CH1			1
#define CMD_CH2			2
#define CMD_ALL			3
#define CMD_TYPE_DC		4
#define CMD_TYPE_AC		5
#define CMD_TYPE_FREQ	6
#define CMD_MODE_VOLT	7
#define CMD_MODE_AMP	8
#define CMD_RUN			9
#define CMD_STOP		10
#define CMD_START		'S'
#define CMD_END			'E'
#define CMD_AUTO		11
#define CMD_INIT		12
#define CMD_GET_STATUS	13
#define CMD_SET_STATUS	14
//#define CMD_DATA		15

 private:
	
	void writeRelayCH1(int r1);
	void writeRelayCH2(int r1);
	void writeRelayCHALL(int r1, int r2);
	void updateStatus();
	int Channel = -1;
	int Type = -1;
	int Mode = -1;
	int Running = -1;
	
	//int RL_DC[5] = { 1, 0, 0, 0, 0 };
	//int RL_AC[5] = { 0, 1, 0, 0, 1 };
	//int RL_AMP[5] = { 0, 0, 1, 1, 0 };
	//int RL_FREQ[5] = { 0, 0, 0, 0, 0 };
	//int RL_OFF[5] = { 0, 0, 0, 0, 0 };	
	
	const unsigned int RL1_DC	= B00000010;
	const unsigned int RL1_AC	= B00100100;
	const unsigned int RL1_AMP	= B00011000;

	const unsigned int RL2_DC	= B00000010;
	const unsigned int RL2_AC	= B00100100;
	const unsigned int RL2_AMP	= B00011000;

	const unsigned int RL_FREQ	= B00000000;
	const unsigned int RL_OFF	= B00000000;

 public:	
	void init();
	void setChannel(int ch);
	void setType(int type);
	void setMode(int mode);
	void setRunning(int running);	
	void setDefault();
	void setFreq();
	void offRelay(int ch);

	int getFreq();
	int getChannel();
	int getType();
	int getMode();
	int getRunning();
	int status[4] = { Channel,Type,Mode,Running };
	String getStatus();
};

extern CMDClass CMD;

#endif


/*
MCGBotEngine header file

Author: Wolfgang Wolff 
Datum: 19.Mai 2012 
Version 0.1
*/

#ifndef MCGBotEngine_h
#define MCGBotEngine_h

// Arduino core Library
#if ARDUINO < 1
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

class MCGBotEngine{

public:
	
	MCGBotEngine(int,int,int,int); // constructor
	
	void setSpeed(int);
	void drive(boolean,int,int);
	void motorRight(boolean, int);
	void motorLeft(boolean, int);

private:

	void digitalWriteEngine(int, boolean, int, boolean);
	long whichPartOfVelocityCurve(long, long, long, int, int);
	
	int xsteppin; // default PIN2 
	int ysteppin; // default PIN6
	int xdirpin;  // default PIN3
	int ydirpin;  // default PIN7
	int spr; // steps per revolution
	
	long ta;
	long td;
	int	Vm;
	int Pt;
	
	long dly;
	long t;
	
	long t12;
	
	int count;
	int Perr; 
};
#endif
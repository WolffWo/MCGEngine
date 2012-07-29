/*
MCGBotEngine cpp file

Author: Wolfgang Wolff Datum: 19.Mai 2012 Version 0.1
*/

// Arduino core Library
#if ARDUINO < 1
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "MCGBotEngine.h"


// public
MCGBotEngine::MCGBotEngine(int pin1, int pin2, int pin3, int pin4) {
	xsteppin = pin1;
	xdirpin = pin2;
	ysteppin = pin3;
    ydirpin = pin4;
	
	pinMode(xsteppin, OUTPUT);
	pinMode(xdirpin, OUTPUT);
	pinMode(ysteppin, OUTPUT);
	pinMode(ydirpin, OUTPUT);
	
}


void MCGBotEngine::drive(boolean dir, int steps, int velocity) {
	// init 
	count = 0;
	Perr = 0;
	// needed date to drive
	ta = 3e6;   // acceleration time 
	td = 3e6;   // deceleration time
	Vm = 3200;  //steady state velocity (pulses/sec)
	Pt = 12800; // max. pulses (distance)
	
	//
	t = 0.0; //ta/100;
	
	// set direction
	digitalWrite(xdirpin, dir);
	digitalWrite(ydirpin, dir);
		
	// drive Pt steps
	for (; ; count++) {
		
		// decide which part of the velocity curve MCGBot is
		dly = whichPartOfVelocityCurve(ta, td, t, Vm, Pt);
	
		t = t + 2*dly;
		
		digitalWriteEngine(xsteppin, HIGH, ysteppin, HIGH); 
		delayMicroseconds(dly);
		digitalWriteEngine(xsteppin, LOW, ysteppin, LOW); 
		delayMicroseconds(dly);	
		
		// finish drive
		if (t>ta+t12+td) {
			// correct for any position error due to rounding
			/*Perr = Pt-count; 
			if (Perr < 0) {
				digitalWriteEngine(xsteppin, 1^digitalRead(xdirpin), ysteppin, 1^digitalRead(ydirpin)); 
				delay(50);
				Perr = -1*Perr;
			}
			for( ;Perr>0;) {
				digitalWriteEngine(xsteppin, HIGH, ysteppin, HIGH); 
				delayMicroseconds(dly);
				digitalWriteEngine(xsteppin, LOW, ysteppin, LOW); 
				delayMicroseconds(dly);	
				Perr--;
			}
			digitalWriteEngine(xsteppin, LOW, ysteppin, LOW); */
			break;
		}	
	}
	
}

//private
void MCGBotEngine::digitalWriteEngine(int xsp, boolean xstate, int ysp, boolean ystate) {
	digitalWrite(xsp,xstate);
	digitalWrite(ysp,ystate);
}

/*
	calculate the time dly corresponding to the time at the velocity function 
*/
long MCGBotEngine::whichPartOfVelocityCurve(long ta, long td, long t,int Vm, int Pt){
	// calculate time at constant velocity
	long t12 = (Pt/(Vm/1e6)) - 0.5*(ta+td);
	// calculate the time-increment at constant velocity
	long dtmin = 1e6/(2*Vm);
	long dtmax = dtmin*3;
	// define local variable
	long dt = dtmax;
	// decide at which part of the velocity function
	int methode = 2; 
	switch(methode) {
	case 1:
	  if (t >= ta/100 && t<ta) { //acceleration part
	  	dt = dtmin*ta/t;
	  }
	  else if (t >= ta && t < (ta+t12)) { // contant velocity part
		dt = dtmin;
	  }
	  else if (t > (ta+t12)){ // deceleration part
		dt = dtmin*ta/(2*ta+t12-t);
  	  }
	  return dt;
	break;
	case 2:
	  if (t >= ta/100 && t<ta) { //acceleration part
		dt = dtmax+(dtmin-dtmax)*t/ta;
	  }
	  else if (t >= ta && t < (ta+t12)) { // contant velocity part
		dt = dtmin;
	  }
	  else if (t >= (ta+t12)){ // deceleration part
		dt = dtmin+(dtmax-dtmin)*(t-ta-t12)/td;
	  }
	  return dt;
	break;
	default:
	  return dtmax;
	break;
	}
}
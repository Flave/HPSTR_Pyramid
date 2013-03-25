/*
	MIDIControl class sends midi notes according to received 
	values. It takes an FSR Object as argument. From it 
	MIDIControll takes the inputValues, sends midiOn once if 
	fsr is pressed and midiOff if released. It also sends 
	controllChange constantly if fsr is pressed.
*/

#ifndef __TouchControl_H__
#define __TouchControl_H__

#include <WProgram.h>
#include "FSR.h"
class FSR;

class TouchControl {

	//boolean to determine whether fsr has been pressed 
	//after beeing released resp. noteOn should be sended
	bool startPlaying;
	
	//boolean to determine whether fsr has been released
	//after beeing pressed resp. noteOff should be sended
	bool stopPlaying;

	public:
		TouchControl ();
		void update(int, FSR, int);
		int velocity;
};

#endif
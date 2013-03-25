#include <WProgram.h>
#include "TouchControl.h"
#include "FSR.h"

TouchControl::TouchControl() {
	startPlaying = true;
	stopPlaying = false;
}


void TouchControl::update(int note, FSR fsr, int channel) {
	int inputValue = fsr.inputValue();
	velocity = map(inputValue, fsr.minValue(), fsr.maxValue(), 0, 127);
	velocity = constrain(velocity, 0, 127);


	//if fsr is pressed
	if(fsr.pressed()) {
		usbMIDI.sendControlChange(20, velocity, channel);
		//Serial.println("FSR Pressed");

		//AND start playing true
		if(startPlaying) {			
			//send midi note
			usbMIDI.sendNoteOn(note, velocity, channel);
			startPlaying = false;
			stopPlaying = true;
		}
	} else if (stopPlaying && !fsr.pressed()) {
		usbMIDI.sendNoteOff(note, 0, channel);
		startPlaying = true;
		stopPlaying = false;
	}
}



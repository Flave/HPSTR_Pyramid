#include <WProgram.h>
#include "TiltControl.h"


TiltControl::TiltControl() {
	previousAmount = 0;
	minVal = 0;
	maxVal = 127;
}

void TiltControl::update(int value, float amount, int channel) {

	float difference = abs(amount - previousAmount);

	if(difference > 0.02) {
		float MIDIValue = int(amount*128);
		 // MIDIValue = map(amount, 0, 100, minVal, maxVal);
		usbMIDI.sendControlChange(value, MIDIValue, channel);
	}
	previousAmount = amount;
}

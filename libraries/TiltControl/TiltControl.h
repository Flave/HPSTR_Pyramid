/*
	Sends Controlchanges based on input Data.
*/

#ifndef __TiltControl_H__
#define __TiltControl_H__

#include <WProgram.h>

class TiltControl {
	bool hasChanged;
	float previousAmount;

	int minVal, maxVal;

	//boolean to determine whether fsr has been pressed 
	//after beeing released resp. noteOn should be sended
	bool amountChanged;

	public:
		TiltControl();
		void update(int, float, int);
		void setMinVal(int val) {minVal = val;}
		void setMaxVal(int val) {maxVal = val;}
};

#endif
/*
  FSR class reads analog values from specified analog pin,
  passes them through inputValue() and sets maxVal and minVal 
  according to the recieived values. Pressed() returns true if 
  sensor is sending.
*/

#ifndef __FSR_H__
#define __FSR_H__

#include <WProgram.h>

class FSR {
	int _pin;
	float _min;
	public:
		FSR();
		FSR (int, int);
		int inputValue();
		bool pressed();
		int maxVal;
		int minVal;
		int maxValue();
		int minValue();

	private:

	};

#endif
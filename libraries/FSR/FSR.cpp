#include <WProgram.h>
#include "FSR.h"

FSR::FSR(int pin, int min) {
	_pin = pin;
	_min = min;
  	maxVal = 150;
  	minVal = 5;
}

FSR::FSR() {};

//function to check weather fsr is pressed or not
bool FSR::pressed() {
	if (inputValue() >= minVal) return true;
	else return false;
}


int FSR::inputValue() {
	int inVal = analogRead(_pin) - _min;
	if(inVal < 0) inVal = 0;
	return inVal;
}

int FSR::maxValue() {
  if(inputValue()>maxVal) maxVal = inputValue();
  return maxVal;
}

int FSR::minValue() {
  return minVal;
}
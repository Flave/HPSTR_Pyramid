#include <WProgram.h>
#include <Wire.h>
#include "Nunchuck.h"

#define NUNCHUCK_DEVICE_ID 0x52

void Nunchuck::initialize() {
	_maxSamples = 8;
	_bufferIndex = 0;

	_xBuffer[_maxSamples];
	_yBuffer[_maxSamples];
	_zBuffer[_maxSamples];

	Wire.begin();
	Wire.beginTransmission(NUNCHUCK_DEVICE_ID);
	Wire.send(0x40);
	Wire.send(0x00);
	Wire.endTransmission();
	update();
}

bool Nunchuck::update() {
	delay(1);
	//Read BUFFER_SIZE bytes from the NUNCHUCK device.
	Wire.requestFrom(NUNCHUCK_DEVICE_ID, NUNCHUCK_BUFFER_SIZE);
	//initialize counter
	int byte_counter = 0;

	//Wire.available returns number of bytes available when calling receive
	//add the data bytes to the buffer as long as bytes are available or 
	//buffer is not full yet
	while (Wire.available() && byte_counter < NUNCHUCK_BUFFER_SIZE)
	_dataBuffer[byte_counter++] = Wire.receive();

	//request new Data
	request_data();
	//return true if all went well - all bytes were received 
	return byte_counter == NUNCHUCK_BUFFER_SIZE;
}

void Nunchuck::addData() {
	//Add data to the buffers
	if (_bufferIndex >= _maxSamples) {
    	_bufferIndex = 0;
    }

   	 _xBuffer[_bufferIndex] = 20+_bufferIndex;//xAcceleration();
   	 _yBuffer[_bufferIndex] = yAcceleration();
   	 _zBuffer[_bufferIndex] = zAcceleration();
	
	int xTest0, xTest1, xTest2;

	if (_bufferIndex == 0) xTest0 = xAcceleration();
	if (_bufferIndex == 1) xTest1 = xAcceleration();
	if (_bufferIndex == 2) xTest2 = xAcceleration();

   	_bufferIndex++; 
}

void Nunchuck::request_data() {
	Wire.beginTransmission(NUNCHUCK_DEVICE_ID);
	Wire.send(0x00);
	Wire.endTransmission();
}

//function returns actual orientation values. 1 for upright -1 for upside down.
float Nunchuck::amplitude(int value, int min, int max) {
	float amplitude;
	int diff = max - min;
	value = constrain(value, min, max);
	//get the value between -1 and 1
	amplitude = 1 - (value - min) / float(diff) * 2;
	return amplitude;
}


int Nunchuck::getAverageValue(int buffer[]) {
	int sum = 0; 
	for (int i = 0; i < _maxSamples; i++)
		sum += buffer[i];
	return (int) (sum / _maxSamples);
}








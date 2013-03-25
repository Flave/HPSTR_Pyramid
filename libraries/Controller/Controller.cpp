#include <WProgram.h>
#include "Controller.h"

void Controller::update(float xVal, float yVal, float zVal) {
	_xVal = xVal;
	_yVal = yVal;
	_zVal = zVal;
}


int Controller::getPeak() {
	//determine if tetraeder is on _peak by reading and adding certain values
	//of the accelerometer
	if(xEdgeVal() + yEdgeVal() + zEdgeVal() >= 2.5) _peak = 1;
	else if(xEdgeVal() + yEdgeVal() <= 0.2 && zEdgeVal() >= 0.80) _peak = 2;
	else if(yEdgeVal() + zEdgeVal() <= 0.2 && xEdgeVal() >= 0.80) _peak = 3;
	else if(xEdgeVal() + zEdgeVal() <= 0.2 && yEdgeVal() >= 0.80) _peak = 4;
	else _peak = 0;

	return _peak;
}


int Controller::getSide() {
	//if Tetraeder is not on _peak, get side tetraeder is on
	if(_peak > 0) {
		_side = 0;
	} else {
		if(xEdgeVal()+yEdgeVal()+zEdgeVal() <= 0.1) _side = 1;
		else if(xEdgeVal() + yEdgeVal() >= 1.9) _side = 2;
		else if(yEdgeVal() + zEdgeVal() >= 1.9) _side = 3;
		else if(xEdgeVal() + zEdgeVal() >= 1.9) _side = 4;
	}
	return _side;
}


bool Controller::sideChanged() {
	bool changed = false;
	if(previousSide != getSide() && !getPeak()) changed = true;
	previousSide = getSide();

	return changed;
}


bool Controller::peakChanged() {
	bool changed = false;
	if(previousPeak != getPeak() && !getSide()) changed = true;
	previousPeak = getPeak();

	return changed;
}


/////////CHANGE MODE FUNCTIONS/////////

//returns true when pyramid was tilted more than specified angle to switch between modes
bool Controller::changeModeX() {
	bool change = false;

	if((xEdgeTilt() < 0.80 && xEdgeTilt() > 0.50) && xModeChanged == false) {
		change = xModeChanged = true;
	}
	if((xEdgeTilt() < 0.50 || xEdgeTilt() > 0.80) && xModeChanged) {
	 xModeChanged = false;
	}

	return change;
}


bool Controller::changeModeY() {
	bool change = false;

	if(yEdgeTilt() < 0.80 && yEdgeTilt() > 0.50 && yModeChanged == false) {
		change = yModeChanged = true;
	}
	if((yEdgeTilt() < 0.50 || yEdgeTilt() > 0.80) && yModeChanged) {
	 yModeChanged = false;
	}

	return change;
}


bool Controller::changeModeZ() {
	bool change = false;

	if(zEdgeTilt() < 0.80 && zEdgeTilt() > 0.50 && zModeChanged == false) {
		change = zModeChanged = true;
	}
	if((zEdgeTilt() < 0.50 || zEdgeTilt() > 0.80) && zModeChanged) {
	 zModeChanged = false;
	}

	return change;
}


////// TILTED AXIS //////

//function to determine which side(axis of accelorameter) is tilted
//for each side edgeTilt() is beeing evaluated for each axis. Dependeing
//on the current side the axises have destinct values if beeing tilted
int Controller::tiltedAxis() {
	int axis;
	switch (getSide()) {
		case 1:
			if(xEdgeVal() < 0) axis = 1;
			if(yEdgeVal() < 0) axis = 2;
			if(zEdgeVal() < 0) axis = 3;
			break;
		case 2:
			if(xEdgeVal() == 1) axis = 1;
			if(yEdgeVal() == 1) axis = 2;
			if(zEdgeVal() < 0) axis = 3;			
			break;
		case 3:
			if(xEdgeVal() < 0) axis = 1;
			if(yEdgeVal() == 1) axis = 2;
			if(zEdgeVal() == 1) axis = 3;
			break;
		case 4:
			if(xEdgeVal() == 0) axis = 1;
			if(yEdgeVal() < 0) axis = 2;
			if(zEdgeVal() == 1) axis = 3;
			break;
	}
	return axis;
}



//function to refine the raw Accelerometer Data to useful Data tailored to the tetraeder
//it returns values between -1 and 1. 1 is returned if axis points upwards when pyramid
//is on one side.
float Controller::edgeVal(float tilt) {
	float value = tilt*1000;
	value = map(value, -570, 570, 0, 100)/100.0;
	value = constrain(value, -1.0, 1.0);
	return value;
}


/////////EDGE TILT FUNCTIONS///////////

float Controller::xEdgeTilt() {
	if(tiltedAxis() == 3) {
		float tilt = edgeVal(_xVal);
		if(_side == 2 || _side == 4) tilt = 1 - tilt;

		//if tilt < 0 it will be 0 values wont be triggered while pyramid is 
		//tilted in an other direction than this one
		if(tilt < 0) tilt = 0;
		
		return tilt;
	} else return 0;
}

float Controller::yEdgeTilt() {
	if(tiltedAxis() == 1) {
		float tilt = edgeVal(_yVal);
		if(_side == 2 || _side == 3) tilt = 1 - tilt;

		if(tilt < 0) tilt = 0;

		return tilt;
	} else return 0;
}

float Controller::zEdgeTilt() {
	
	if(tiltedAxis() == 2) {
		float tilt = edgeVal(_zVal);
		if(_side == 3 || _side == 4) tilt = 1 - tilt;

		if(tilt < 0) tilt = 0;

		return tilt;
	} else return 0;
}


/////////SLOW CHANGE FUNCTIONS/////////

float Controller::xSlowChanged(float max) {

	//only evaluate edgeVal if pyramid is tilted over zAxis
	if(tiltedAxis() == 3) {

		//get current tilt value
		float tilt = edgeVal(_xVal);

		//set tilt value according to current side so the returned value will 
		//always be 0 when pyramid is not tilted
		if(_side == 2 || _side == 4) tilt = 1 - tilt;	

		if(tilt < 0) tilt = 0;
		float difference = abs(tilt-previousXTilt);

		//the bigger max the faster you have to tilt
		if(difference > max) {
			//return cached value if the difference is bigger than max
			
			previousXTilt = tilt;
			return xCache;
		}
		//return current value if difference is smaller than max
		//also set cache and previousValue to tilt for next loop
		xCache = tilt;
		previousXTilt = tilt;

		return tilt;
	} else return 0;
}


float Controller::ySlowChanged(float max) {
	if(tiltedAxis() == 1) {
		//get current tilt value
		float tilt = edgeVal(_yVal);

		//set tilt value according to current side so the returned value will 
		//always be 0 when pyramid is not tilted
		if(_side == 2 || _side == 3) tilt = 1 - tilt;

		if(tilt < 0) tilt = 0;
		float difference = abs(tilt-previousYTilt);

		if(difference > max) {
			//return cached value if the difference is bigger than max
			previousYTilt = tilt;
			return yCache;
		}

		//return current value if difference is smaller than max
		//also set cache and previousValue to tilt for next loop
		yCache = tilt;
		previousYTilt = tilt;

		return tilt;
	} else return 0;
}


float Controller::zSlowChanged(float max) {

	if(tiltedAxis() == 2) {
		//get current tilt value
		float tilt = edgeVal(_zVal);

		//set tilt value according to current side so the returned value will 
		//always be 0 when pyramid is not tilted
		if(_side == 3 || _side == 4) tilt = 1 - tilt;
		
		if(tilt < 0) tilt = 0;
		float difference = abs(tilt-previousZTilt);

		if(difference > max) {
			//return cached value if the difference is bigger than max
			previousZTilt = tilt;
			return zCache;
		}

		//return current value if difference is smaller than max
		//also set cache and previousValue to tilt for next loop
		zCache = tilt;
		previousZTilt = tilt;

		return tilt;
	} else return 0;
}
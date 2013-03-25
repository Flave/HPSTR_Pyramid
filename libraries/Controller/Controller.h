/*
	Controller determines the Orientation of the Tetraeder based on
	inputData of an Accelerometer. It sets side and peak according
	to received Data.
*/

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <WProgram.h>

class Controller {
	
	public:

		//returns values between -1 and 1
		float xEdgeVal() {
			return edgeVal(_xVal);
		}

		float yEdgeVal() {
			return edgeVal(_yVal);
		}

		float zEdgeVal() {
			return edgeVal(_zVal);
		}

		void update(float, float, float);
		int getPeak();
		int getSide();

		//returns true if changed from side to side or from peak to side
		bool sideChanged();

		//returns true if changed from peak to peak or from side to peak
		bool peakChanged();

		//returns the axis the pyramid is beeing tilted
		//1=x 2=y 3=z
		//used to prevent [...]edgeTilt() from returning values if pyramid is
		//not tilted on the intended side
		int tiltedAxis();
		int previousSide;
		int previousPeak;

		//return values from 0 to 1 depending on which axis is being tilted
		//returns 0 if standing on side
		//returns 1 if fully tilted
		float xEdgeTilt();
		float yEdgeTilt();
		float zEdgeTilt();

		//function used to determine tilt-values under a certain speed
		//basically the same as [...]edgeTilt() but returns a cached (previous) value
		//if tilted faster than with indicated speed
		float xSlowChanged(float);
		float ySlowChanged(float);
		float zSlowChanged(float);


		//additional tilt gesture
		bool changeModeX();
		bool changeModeY();
		bool changeModeZ();

		bool xModeChanged;
		bool yModeChanged;
		bool zModeChanged;


	private:
		//variables holding the orientation of the pyramid
		int _side, _peak;

		//returns the accelerometer mapped from -0.57 - 0.57 to 0 - 1
		float edgeVal(float);

		//variables holding the accelerometer values which are received in update() 
		float _xVal, _yVal, _zVal;

		//variables for calculating the change in values between two loops
		//previous[...] holds the actual previous value received from accelerometer and is 
		//used for calculating the absolute change to previous loop
		float previousXTilt, previousYTilt, previousZTilt;

		//cache holds the last value returned by [...]SlowChanged
		float xCache, yCache, zCache;
};

#endif
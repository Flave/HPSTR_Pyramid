#ifndef __NUNCHUCK_H__
#define __NUNCHUCK_H__

#define NUNCHUCK_BUFFER_SIZE 6

class Nunchuck {
	void request_data();
	unsigned char _dataBuffer[NUNCHUCK_BUFFER_SIZE];

	float amplitude(int, int, int);
	int getAverageValue(int[]);

	static const int xMin = 384;
	static const int xMax = 608;
	static const int yMin = 400;
	static const int yMax = 624;
	static const int zMin = 425;
	static const int zMax = 623;

	int _maxSamples;
	int _bufferIndex;

	int _xBuffer[8];
	int _yBuffer[8];
	int _zBuffer[8];

	public:
		void initialize();
		bool update();
		void addData();
		int joystick_x() const { return _dataBuffer[0]; }
		int joystick_y() const { return _dataBuffer[1]; }

		//functions returning actual Acceleration
		int xAcceleration() const {
			return ((int)(_dataBuffer[2]) << 2) | ((_dataBuffer[5] >> 2) & 0x03);
		}

		int yAcceleration() const {
			return ((int)(_dataBuffer[3]) << 2) | ((_dataBuffer[5] >> 4) & 0x03);
		}
	
		int zAcceleration() const {
			return ((int)(_dataBuffer[4]) << 2) | ((_dataBuffer[5] >> 6) & 0x03);
		}


		//functions returning flattened Acceleration Values
		int xAccelerationFlat() {
			return getAverageValue(_xBuffer);
		}

		int yAccelerationFlat() {
			return getAverageValue(_yBuffer);
		}

		int zAccelerationFlat() {
			return getAverageValue(_zBuffer);
		}


		//functions returning Amplitude/ Orientation vectors
		float xAmplitude() {
			return amplitude(xAcceleration(), xMin, xMax);
		}

		float yAmplitude() {
			return amplitude(yAcceleration(), yMin, yMax);
		}

		float zAmplitude() {
			return amplitude(zAcceleration(), zMin, zMax);
		}


		//functions returning Amplitude/ Orientation vectors flattened
		float xAmplitudeFlat() {
			return amplitude(xAccelerationFlat(), xMin, xMax);
		}

		float yAmplitudeFlat() {
			return amplitude(yAccelerationFlat(), xMin, xMax);
		}

		float zAmplitudeFlat() {
			return amplitude(zAccelerationFlat(), xMin, xMax);
		}


	
		bool z_button() const { return !(_dataBuffer[5] & 0x01); }
		bool c_button() const { return !(_dataBuffer[5] & 0x02); }
	};

	#endif
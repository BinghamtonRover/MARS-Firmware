#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include "../gps.pb.h"

#define GPS_BAUD 9600
#define GPS_SERIAL Serial4

class Gps {
	private:
		TinyGPSPlus gps;

	public:
		Gps(int rxPin, int txPin);

		void setup();
		void waitForFix();
		GpsCoordinates getCoordinates();
		GpsCoordinates getAverageReading(int numSamples);
};

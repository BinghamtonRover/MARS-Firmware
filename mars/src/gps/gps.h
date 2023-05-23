#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include "../gps.pb.h"

#define GPS_BAUD 9600

class Gps {
	private:
		TinyGPSPlus gps;
		SoftwareSerial softwareSerial;

	public:
		Gps(int rxPin, int txPin);

		void setup();
		GpsCoordinates getCoordinates();
		GpsCoordinates getStationaryCoordinates();
};

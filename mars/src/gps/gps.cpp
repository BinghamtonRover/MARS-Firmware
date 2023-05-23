#include "gps.h"

void Gps::setup() {
	Serial.begin(9600);
	GPS_SERIAL.begin(GPS_BAUD);
}

void Gps::waitForFix() {
	while (!gps.date.isValid()) {
		while (GPS_SERIAL.available() > 0) {
			gps.encode(GPS_SERIAL.read());
		}
	}
}

GpsCoordinates Gps::getCoordinates() {
	while(GPS_SERIAL.available() > 0) {
		gps.encode(GPS_SERIAL.read());
		if (!gps.location.isValid()) continue;
		return {
			latitude: (float) gps.location.lat(),
			longitude: (float) gps.location.lng(),
			altitude: (float) gps.altitude.meters(),
		};
	}
	if (gps.charsProcessed() < 10) {
		Serial.println(F("No GPS detected: check wiring."));    
		while (true);
	}
	return GpsCoordinates_init_zero;
}

GpsCoordinates Gps::getAverageReading(int numSamples) {
	long longitudeSum = 0, latitudeSum = 0, altitudeSum = 0;
	for (int i = 0; i < numSamples; i++) {
		GpsCoordinates reading = getCoordinates();
		longitudeSum += reading.longitude;
		latitudeSum += reading.latitude;
		altitudeSum += reading.altitude;
		delay(10);
	}
	return {
		latitude: (float) latitudeSum / numSamples, 
		longitude: (float) longitudeSum / numSamples, 
		altitude: (float) altitudeSum / numSamples
	};
}

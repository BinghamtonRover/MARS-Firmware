#include "gps.h"

Gps::Gps(int rxPin, int txPin); //  : softwareSerial(SoftwareSerial(rxPin, txPin)) { }

void Gps::setup() {
	Serial.begin(9600);
	Serial4.begin(GPS_BAUD);
}

void waitForFix() {
	while (!gps.date.isValid()) {
		while (Serial4.available() > 0) {
			gps.encode(Serial4.read());
		}
	}
}

GpsCoordinates Gps::getCoordinates() {
	GpsCoordinates result = {-1, -1, -1};
	while(Serial4.available() > 0) {
		gps.encode(Serial4.read());
		if (!gps.location.isValid()) continue;
		return {
			x: gps.location.lat(),
			y: gps.location.lng(),
			z: gps.location.altitude.meters(),
		}
	}
	if (gps.charsProcessed < 10) {
		Serial.println(F("No GPS detected: check wiring."));    
		while (true);
	}
	return GpsCoordinates_init_zero;
}

GpsCoordinates gps::getAverageReading(int numSamples) {
	long sumX, sumY, sumZ;
	for (int i = 0; i < numSamples; i++) {
		GpsCoordinates reading = getCoordinates();
		sumX += reading.x;
		sumY += reading.y;
		sumZ += reading.z;
	}
	return {
		x: sumX / numSamples, 
		y: sumY / numSamples, 
		z: sumZ / numSamples
	};
}

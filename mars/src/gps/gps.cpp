#include "gps.h"

Gps::Gps(int rxPin, int txPin) { } //  : softwareSerial(SoftwareSerial(rxPin, txPin)) { }

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
	while (true) {
		while(GPS_SERIAL.available() > 0) {
			if (gps.encode(GPS_SERIAL.read()) && gps.location.isValid() && gps.location.isUpdated()) {
				Serial.print(": lat=");
				Serial.print(gps.location.lat(), 10);
				Serial.print(", long=");
				Serial.print(gps.location.lng(), 10);
				Serial.print(", alt=");
				Serial.println(gps.altitude.meters(), 10);			
				return {
					latitude: (float) gps.location.lat(),
					longitude: (float) gps.location.lng(),
					altitude: (float) gps.altitude.meters(),
				};
			}
		}
	}
}

GpsCoordinates Gps::getAverageReading(int numSamples) {
	long double longitudeSum = 0;
	long double latitudeSum = 0;
	long double altitudeSum = 0;
	for (int i = 0; i < numSamples; i++) {
		Serial.print("Reading #");
		Serial.print(i);
		GpsCoordinates reading = getCoordinates();
		longitudeSum += reading.longitude;
		latitudeSum += reading.latitude;
		altitudeSum += reading.altitude;
		delay(1000);
	}
	return {
		latitude: (float) (latitudeSum / numSamples), 
		longitude: (float) (longitudeSum / numSamples), 
		altitude: (float) (altitudeSum / numSamples),
	};
}

#include <math.h>

#include "src/utils/BURT_utils.h"

#include "src/gps.pb.h"
#include "src/mars.pb.h"
#include "pinouts.h"

#define THRESHOLD_SWIVEL  0.0                 // degrees
#define THRESHOLD_TILT    0.0                 // degrees
#define SEND_DATA_INTERVAL 1000
#define GPS_SAMPLE_COUNT 10

GpsCoordinates baseStation = GpsCoordinates_init_zero;
unsigned long nextSendTime;

void handleCommand(const uint8_t* data, int length);
BurtSerial serial(handleCommand, Device::Device_MARS);

void setup() {
  delay(1000);
  Serial.println("Initializing MARS subsystem...");

  Serial.println("Initializing motors...");
  swivel.presetup();
  tiltNegative.presetup();
  tiltPositive.presetup();

  swivel.setup();
  tiltPositive.setup();
  tiltNegative.setup();

  Serial.print("Initializing GPS, waiting for fix... ");
  gps.setup();
  gps.waitForFix();
  Serial.print("Got a fix... ");
  delay(1000);
  baseStation = gps.getAverageReading(GPS_SAMPLE_COUNT);
  Serial.println("Done!");

  Serial.print("Base station coordinates: lat=");
  Serial.print(baseStation.latitude, 10);
  Serial.print(", long=");
  Serial.print(baseStation.longitude, 10);
  Serial.print(", alt=");
  Serial.println(baseStation.altitude, 10);

  Serial.println("MARS subsystem initialized.");
  nextSendTime = millis() + SEND_DATA_INTERVAL;
} 

void loop() {
  // Update motors
  swivel.update();
  /* DISABLED: Motors will break!
  tiltPositive.update();
  tiltNegative.update();
  */

  // Update communications
  serial.update();
  sendData();
}

float getAngleSwivel(GpsCoordinates coordinates) {
  float diffx = coordinates.latitude - baseStation.latitude;
  float diffy = coordinates.longitude - baseStation.longitude;
  return atan2(diffy,diffx);
} 

float getAngleTilt(GpsCoordinates coordinates) {
  float diffx = coordinates.latitude - baseStation.latitude;
  float diffy = coordinates.longitude - baseStation.longitude;
  float diffz = coordinates.altitude - baseStation.altitude;
  float distance = sqrt(pow(diffx, 2) + pow(diffy, 2)); 
  return atan2(distance, diffz);
}

void handleCommand(const uint8_t* data, int length) {
  auto command = BurtProto::decode<MarsCommand>(data, length, GpsCoordinates_fields);

  // Move the motors manually
  swivel.moveBy(command.swivel);
  /* DISABLED: The bands around the tilt will break
  tiltPositive.moveBy(command.tilt);
  tiltNegative.moveBy(-command.tilt);
  */

  // Update the base station coordinates
  if (command.has_baseStationOverride) baseStation = command.baseStationOverride;

  // Update the rover's position and move the motors
  if (command.has_position) {
    // Swivel to face the rover
    float angle_swivel = getAngleSwivel(command.position);
    if(abs(angle_swivel - swivel.getPosition()) >= THRESHOLD_SWIVEL) {
      swivel.moveTo(angle_swivel);
    }

    // Tilt to face the rover
    /* DISABLED: The bands around the tilt motors will break
    float angle_tilt = getAngleTilt(command.position);
    if(abs(angle_tilt - titlPos.angle) >= THRESHOLD_TILT) {
      tiltPositive.moveTo(angle_tilt);
      tiltNegative.moveTo(-angle_tilt);
    }
    */
  }
}

void sendData() {
  if (millis() < nextSendTime) return;
  MarsData data = {
    swivel: swivel.getPosition(),
    tilt: tiltPositive.getPosition(),
    coordinates: baseStation,
  };
  serial.send(MarsData_fields, &data, MarsData_size);
}

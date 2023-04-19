#include "src/tmc/BURT_TMC.h"
#include "src/utils/BURT_utils.h"
#include "src/gps.pb.h"

#include <math.h>

#define THRESHOLD_SWIVEL 10
#define THRESHOLD_TILT 2

StepperMotor swivel(10, 29, 0, 1500, -PI, PI, 100, 324, 162, "Swivel");
StepperMotor tiltneg(1, 2, 3, 4, 5, 6, 7, 8, 9, "TiltNeg");
StepperMotor tiltpos(1, 2, 3, 4, 5, 6, 7, 8, 9, "TiltPos");

GpsCoordinates currentposition;

void handler(const uint8_t* data, int length) {
  // do something with the data
  auto coordinates = BurtProto::decode<GpsCoordinates>(data, length, GpsCoordinates_fields);
  float angleswivel = getAngleSwivel(coordinates);
  float anglez = getAngleTilt(coordinates);
  if(angleswivel>= THRESHOLD_SWIVEL) {
   swivel.moveBy(angleswivel);
   tiltpos.moveBy(anglez);
   tiltneg.moveBy(-1*anglez);
  currentposition.longitude = coordinates.longitude;
  currentposition.latitude = coordinates.latitude;
  }
  if(anglez>= THRESHOLD_TILT){
     tiltpos.moveBy(anglez);
   tiltneg.moveBy(-1*anglez);
  currentposition.altitude = coordinates.altitude;
  }
  // ...
}

BurtSerial serial(handler, Device::Device_MARS);

float getAngleSwivel(GpsCoordinates coordinates) {
  float diffx, diffy, angle;
  diffx = coordinates.longitude - currentposition.longitude;
  diffy = coordinates.latitude - currentposition.latitude;
  angle = atan2(diffy,diffx);
  return angle;
 } 

float getAngleTilt(GpsCoordinates coordinates){
  float diffx, diffy, diffz, anglez, distance;
  diffx = coordinates.longitude - currentposition.longitude;
  diffy = coordinates.latitude - currentposition.latitude;
  diffz = coordinates.altitude - currentposition.altitude;
  distance = sqrt(pow(diffx, 2) + pow(diffy, 2)); 
  anglez = atan2(distance, diffz) +4;
  return anglez;
 }

void setup() { 
  tiltneg.presetup();
  swivel.presetup();
  tiltpos.presetup();

  swivel.setup();
  tiltpos.setup();
  tiltneg.setup();

  swivel.moveBy(PI);
  delay(2000);
  swivel.moveBy(-PI);
} 

void loop() {
  swivel.update();
  tiltpos.update();
  tiltneg.update();

  serial.update();
  delay(10);
}

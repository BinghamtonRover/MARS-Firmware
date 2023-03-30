#include "src/tmc/BURT_TMC.h"
#include "src/utils/BURT_utils.h"
#include "src/gps.pb.h"

#include <math.h>

#define THRESHOLD_SWIVEL 10
#define THRESHOLD_TILT 2

StepperMotor swivel(10, 29, 0, 1500, -PI, PI, 324, 324, 162, "Swivel");
StepperMotor tiltneg(1, 2, 3, 4, 5, 6, 7, 8, 9, "TiltNeg");
StepperMotor tiltpos(1, 2, 3, 4, 5, 6, 7, 8, 9, "TiltPos");

GPSCoordinates currentposition = {0};

void handler(const uint8_t* data, int length) {
  // do something with the data
  //auto coordinates = BurtProto::decode<GpsCoordinates>(data, length, GpsCoordinates_fields);
  GpsCoordinates coordinates = {100, 100, 100};
  float angleswivel = getAngleSwivel(coordinates);
  float anglez = getAngleTilt(coordinates);
  if(angleswivel>= THRESHOLD_SWIVEL) {
   swivel.moveBy(angleswivel);
   tiltpos.moveBy(anglez);
   tiltneg.moveBy(-1*anglez);
  currentposition.x = coordinates.x;
  currentposition.y = coordinates.y;
  }
  if(anglez>= THRESHOLD_TILT){
     tiltpos.moveBy(anglez);
   tiltneg.moveBy(-1*anglez);
  currentposition.z = coordinates.z;
  }
  // ...
}

BurtSerial serial(handler, Device::MARS);

float getAngleSwivel(GpsCoordinates coordinates) {
  float diffx, diffy, angle;
  diffx = coordinates.x - currentposition.x;
  diffy = coordinates.y - currentposition.y;
  angle = atan2(diffy,diffx);
  return angle;
 } 

 float getAngleTilt(GpsCoordinates coordinates){
 float diffx, diffy, diffz, anglez, distance;
 diffx = coordinates.x - currentposition.x;
 diffy = coordinates.y - currentposition.y;
 diffz = coordinates.z - currentposition.z;
 distance = sqrt((diffx)^2 + (diffy)^2); 
 anglez = atan2(distance, diffz) +4;
 return anglez;

 }

void setup() { 
  swivel.presetup();
  swivel.setup();
  tiltpos.presetup();
  tiltpos.setup();
  tiltneg.presetup();
  tiltneg.setup();
} 

void loop() {
  swivel.update();
  tiltpos.update();
  tiltneg.update();
  serial.update();
  delay(10);
}

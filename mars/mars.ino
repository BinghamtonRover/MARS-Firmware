#include "src/tmc/BURT_TMC.h"
#include "src/utils/BURT_utils.h"
#include "src/gps.pb.h"
#include "pin_select.h"
#include <math.h>

#define THRESHOLD_SWIVEL 10
#define THRESHOLD_TILT 2

GpsCoordinates currentposition;

void handler(const uint8_t* data, int length) {
  // do something with the data
  auto coordinates = BurtProto::decode<GpsCoordinates>(data, length, GpsCoordinates_fields);
  float angleswivel = getAngleSwivel(coordinates);
  float anglez = getAngleTilt(coordinates);
  if(angleswivel>= THRESHOLD_SWIVEL) {
   swivel.moveBy(angleswivel);
  tiltPos.moveBy(anglez);
  tiltNeg.moveBy(-1*anglez);
  currentposition.longitude = coordinates.longitude;
  currentposition.latitude = coordinates.latitude;
  }
  if(anglez>= THRESHOLD_TILT){
  tiltPos.moveBy(anglez);
  tiltNeg.moveBy(-1*anglez);
  currentposition.altitude = coordinates.altitude;
  }
  
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
 tiltNeg.presetup();
  swivel.presetup();
  tiltPos.presetup();

  swivel.setup();
  tiltPos.setup();
 tiltNeg.setup();
  tiltPos.moveBy(0);
  tiltNeg.moveBy(0);
  //swivel.moveBy(PI);
  delay(2000);
  //swivel.moveBy(-PI);
} 

void loop() {
  swivel.update();
  tiltPos.update();
  tiltNeg.update();

  serial.update();
  delay(10);
}

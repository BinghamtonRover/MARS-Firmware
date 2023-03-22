#include "tmc/src/BURT_TMC.h"
#include "utils/src/BURT_serial.h"
#include <math.h>
#define THRESHOLD 10
//#define PI 3.14159
StepperMotor swivel(10, 29, 0, 1500, -PI, Pi, 324, 324, 162, "Swivel");
//StepperMotor tiltneg(1, 2, 3, 4, 5, 6, 7, 8, 9, "Motor2");
//StepperMotor tiltpos(1, 2, 3, 4, 5, 6, 7, 8, 9, "Tilt");
BurtSerial serial(handler);

float getAngle(GpsCoordinates coordinates) {
  float diffx, diffy, anglerad, angle;
  diffx = GpsCoordinates.x - coordinates.x;
  diffy = GpsCoordinates.y - coordinates.y;
  angle = atan2(diffy,diffx);
  //angle = anglerad*PI/180;
  return angle
 } 

void handler(const uint8_t* data, int length) {
  // do something with the data
  auto coordinates = BurtProto::decode<GpsCoordinates>(data, length, GpsCoordinates_fields);
  float angle = getAngle(coordinates);
  if(angle>= THRESHOLD) {
   swivel.moveBy(angle);
  }
  // ...
}

void setup() { 
  swivel.presetup();
  swivel.setup();
 // motor2.presetup();
 // motor2.setup();
} 

void loop() {
  swivel.update();
  serial.update();
  delay(10);
}

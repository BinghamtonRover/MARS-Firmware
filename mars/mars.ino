#include "src/tmc/BURT_TMC.h"
#include "src/utils/BURT_utils.h"

struct GpsCoordinates { };

StepperMotor motor(1, 2, 3, 4, 5, 6, 7, 8, 9, "Motor");
BurtSerial serial(handler);

float getAngle(GpsCoordinates coordinates) { } 

void handler(const uint8_t* data, int length) {
  // do something with the data
  // auto coordinates = BurtProto::decode<GpsCoordinates>(data, length, GpsCoordinates_fields);
  GpsCoordinates coordinates;
  float angle = getAngle(coordinates);
  motor.moveTo(angle);
  // ...
}

void setup() { 
  motor.presetup();
  motor.setup();
} 

void loop() {
  motor.update();
  serial.update();
  delay(10);
}

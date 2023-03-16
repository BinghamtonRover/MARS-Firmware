#include "tmc/src/BURT_TMC.h"
#include "utils/src/BURT_serial.h"

StepperMotor motor(1, 2, 3, 4, 5, 6, 7, 8, 9, "Motor");
BurtSerial serial(handler);

float getAngle(GpsCoordinates coordinates) { } 

void handler(const uint8_t* data, int length) {
  // do something with the data
  auto coordinates = BurtProto::decode<GpsCoordinates>(data, length, GpsCoordinates_fields);
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

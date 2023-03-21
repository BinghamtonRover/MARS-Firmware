#include "src/tmc/BURT_TMC.h"
#include "src/utils/BURT_utils.h"

void handler(const uint8_t* data, int length) { }
BurtCan can(0, handler);
BurtSerial serial(handler);
StepperMotor motor(1, 2, 3, 4, 5, 6, 7, 8, 9, "Motor");

void setup() { }
void loop() { }

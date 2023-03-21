// #include "tmc/src/BURT_TMC.h"
#include "src/utils/BURT_utils.h"

void handler(const uint8_t* data, int length) { }
BurtCan can(0, handler);
BurtSerial serial(handler);

void setup() { }
void loop() { }

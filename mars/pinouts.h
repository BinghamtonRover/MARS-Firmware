#include "src/tmc/BURT_TMC.h"
#include "src/gps/gps.h"

#define SPEED 0x000327e7
#define ACCEL 0x00030d4d

#define GPS_RX 16
#define GPS_TX 17

Gps gps(GPS_RX, GPS_TX);

StepperMotorPins swivelPins = {
	chipSelect: 10,
	enable: 35,
};

StepperMotorConfig swivelConfig = {
	name: "swivel",
	current: 1500,
	speed: SPEED,
	accel: ACCEL,
	minLimit: -3 * PI/4,
	maxLimit: 3 * PI/4,
	isPositive: true,
	gearboxRatio: 10.25,
};

LimitSwitch swivelLimitSwitch;

StepperMotor swivel(swivelPins, swivelConfig, swivelLimitSwitch);

// =============================================

StepperMotorPins tiltPositivePins = {
	chipSelect: 37,
	enable: 34,
};

StepperMotorConfig tiltPositiveConfig = {
	name: "tiltPos",
	current: 2000,
	speed: SPEED,
	accel: ACCEL,
	minLimit: -10*PI,
	maxLimit: 10*PI,
	isPositive: true,
	gearboxRatio: 8,
};

LimitSwitch tiltPositiveLimitSwitch;

StepperMotor tiltPositive(tiltPositivePins, tiltPositiveConfig, tiltPositiveLimitSwitch);

// =============================================

StepperMotorPins tiltNegativePins = {
	chipSelect: 36,
	enable: 40,
};

StepperMotorConfig tiltNegativeConfig = {
	name: "tiltNeg",
	current: 2000,
	speed: SPEED,
	accel: ACCEL,
	minLimit: -10*PI,
	maxLimit: 10*PI,
	isPositive: false,
	gearboxRatio: 8,
};

LimitSwitch tiltNegativeLimitSwitch;

StepperMotor tiltNegative(tiltNegativePins, tiltNegativeConfig, tiltNegativeLimitSwitch);

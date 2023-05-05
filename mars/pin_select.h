#include "src/tmc/BURT_TMC.h"

#define SPEED 0x000327e7
#define ACCEL 0x00030d4d

StepperMotorPins swivelPins = {
    chipSelect: 10,
    enable: 35,
    limitSwitch: 0,
};

StepperMotorConfig swivelConfig = {
    name: "swivel",
    current: 2000,
    limitSwitchPosition: 0,
    minLimit: -PI,
    maxLimit: PI,
    isPositive: true,
    gearboxRatio: 5,
    speed: SPEED,
    accel: ACCEL,
};

StepperMotor swivel(swivelPins, swivelConfig);

// =============================================

StepperMotorPins tiltPosPins = {
    chipSelect: 37,
    enable: 34,
    limitSwitch: 0,
};

StepperMotorConfig tiltPosConfig = {
    name: "tiltPos",
    current: 2000,
    limitSwitchPosition: 0,
    minLimit: 0,
    maxLimit: 1.71042,
    isPositive: true,
    gearboxRatio: 1,
    speed: SPEED,
    accel: ACCEL,
};

StepperMotor tiltPos(tiltPosPins, tiltPosConfig);

// =============================================

StepperMotorPins tiltNegPins = {
    chipSelect: 36,
    enable: 40,
    limitSwitch: 0,
};

StepperMotorConfig tiltNegConfig = {
    name: "tiltNeg",
    current: 2000,
    limitSwitchPosition: 0,
    minLimit: -PI,
    maxLimit: PI,
    isPositive: true,
    gearboxRatio: 1,
    speed: SPEED,
    accel: ACCEL,
};

StepperMotor tiltNeg(tiltNegPins, tiltNegConfig);
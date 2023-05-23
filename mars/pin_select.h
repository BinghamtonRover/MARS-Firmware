#include "src/tmc/BURT_TMC.h"

#define SPEED 0x000327e7
#define ACCEL 0x00030d4d

StepperMotorPins swivelPins = {
    chipSelect: 10,
    enable: 35,
    limitSwitch: 0
};

StepperMotorConfig swivelConfig = {
    name: "swivel",
    current: 1500,
    limitSwitchPosition: 0,
    minLimit: -130.0*PI/180.0,
    maxLimit: 130.0*PI/180.0,
    isPositive: true,
    gearboxRatio: 10,
    speed: SPEED,
    accel: ACCEL
};

StepperMotor swivel(swivelPins, swivelConfig);

// =============================================

StepperMotorPins tiltPosPins = {
    chipSelect: 37,
    enable: 34,
    limitSwitch: 0
};

StepperMotorConfig tiltPosConfig = {
    name: "tiltPos",
    current: 2000,
    limitSwitchPosition: 0,
    minLimit: -10*PI,
    maxLimit: 10*PI,
    isPositive: true,
    gearboxRatio: 8,
    speed: SPEED,
    accel: ACCEL
};

StepperMotor tiltPos(tiltPosPins, tiltPosConfig);

// =============================================

StepperMotorPins tiltNegPins = {
    chipSelect: 36,
    enable: 40,
    limitSwitch: 0
};

StepperMotorConfig tiltNegConfig = {
    name: "tiltNeg",
    current: 2000,
    limitSwitchPosition: 0,
    minLimit: -10*PI,
    maxLimit: 10*PI,
    isPositive: false,
    gearboxRatio: 8,
    speed: SPEED,
    accel: ACCEL
};

StepperMotor tiltNeg(tiltNegPins, tiltNegConfig);

#include "src/tmc/BURT_TMC.h"
#include "src/utils/BURT_utils.h"
#include "src/gps.pb.h"
#include "pin_select.h"
#include <math.h>

#define THRESHOLD_SWIVEL  0.0                 // degrees
#define THRESHOLD_TILT    0.0                 // degrees

#define INIT_COORDS       { 0.0, 0.0, 0.0 }   // { latitude, longitude, altitude }

// comment to disable debug
// #define DEBUG_ANGLE // enter in an angle to move swivel and/or tilt
#define DEBUG_COORDS // enter in coordinates to move swivel and tilt

GpsCoordinates local_coords = INIT_COORDS;
#ifdef DEBUG_COORDS
GpsCoordinates debug_coords = INIT_COORDS;
uint8_t debug_count = 1;
#endif
void handler(const uint8_t* data, int length);
#if !defined(DEBUG_ANGLE) && !defined(DEBUG_COORDS)
BurtSerial serial(handler, Device::Device_MARS);
#endif

void handler(const uint8_t* data, int length)
{
  auto coordinates = BurtProto::decode<GpsCoordinates>(data, length, GpsCoordinates_fields);
  float angle_swivel = getAngleSwivel(coordinates);
  float angle_tilt = getAngleTilt(coordinates);

  if(abs(angle_swivel - swivel.angle) >= THRESHOLD_SWIVEL)
  {
    swivel.moveTo(angle_swivel);
  }

  if(abs(angle_tilt - titlPos.angle) >= THRESHOLD_TILT)
  {
    tiltPos.moveTo(angle_tilt);
    tiltNeg.moveTo(angle_tilt);
  } 
}

// place holder
GpsCoordinates getLocalCoords()
{
  GpsCoordinates coords = { 0, 0, 0 };
  return coords;
}

float getAngleSwivel(GpsCoordinates coordinates)
{
  float diffx, diffy;
  diffx = coordinates.latitude - local_coords.latitude;
  diffy = coordinates.longitude - local_coords.longitude;
  return atan2(diffy,diffx);
} 

float getAngleTilt(GpsCoordinates coordinates)
{
  float diffx, diffy, diffz, distance;
  diffx = coordinates.latitude - local_coords.latitude;
  diffy = coordinates.longitude - local_coords.longitude;
  diffz = coordinates.altitude - local_coords.altitude;
  distance = sqrt(pow(diffx, 2) + pow(diffy, 2)); 
  return atan2(distance, diffz);
 }

void setup()
{ 
  tiltNeg.presetup();
  swivel.presetup();
  tiltPos.presetup();

  swivel.setup();
  tiltPos.setup();
  tiltNeg.setup();

  delay(2000);

  local_coords = getLocalCoords();
} 

void loop()
{
  swivel.update();
  tiltPos.update();
  tiltNeg.update();
  #if !defined(DEBUG_ANGLE) && !defined(DEBUG_COORDS)
  serial.update();
  #endif

  #if defined(DEBUG_ANGLE)
  if(Serial.available())
  {
    float angle = PI/180.0*Serial.parseInt();
    // comment out to disable motion of motors
    swivel.moveTo(angle);
    // tiltPos.moveTo(angle);
    // tiltNeg.moveTo(angle);
  }
  #elif defined(DEBUG_COORDS)
  if(Serial.available())
    {
      switch(debug_count)
      {
        default:
        case 1:
          debug_coords.latitude = Serial.parseFloat();
          debug_count++;
          break;
        case 2:
          debug_coords.longitude = Serial.parseFloat();
          debug_count++;
          break;
        case 3:
          debug_coords.altitude = Serial.parseFloat();
          float angle_swivel = getAngleSwivel(debug_coords);
          float angle_tilt = getAngleTilt(debug_coords);

          if(abs(angle_swivel - current_angle_swivel) >= THRESHOLD_SWIVEL)
          {
            swivel.moveTo(angle_swivel);
            current_angle_swivel+= angle_swivel;
          }

          if(abs(angle_tilt - current_angle_tilt) >= THRESHOLD_TILT)
          {
            tiltPos.moveTo(angle_tilt);
            tiltNeg.moveTo(angle_tilt);
            current_angle_tilt+= angle_tilt;
          }
          debug_count = 1;
          break;
      }
    }
  #endif

  delay(10);
}

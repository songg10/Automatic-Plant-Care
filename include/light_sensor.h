// Header defining functions to read the value of the light sensor (in Lumens).

#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

// Returns the raw sensor voltage reading
int Light_getRawVoltage();

// Convert the voltage reading into lumens (unit for measuring light)
double Light_getLumens ();

#endif 
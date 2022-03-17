// Header defining functions to read the value of the light sensor (in Lumens).

#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

// Returns the raw sensor voltage reading
int Light_getRawVoltage();

// Convert the voltage reading into lumens (unit for measuring light)
double Light_getLumens ();

// Set the minimum safety threshold for light level
void Light_setMinThreshold(int min);

// Get the minimum safety threshold for light level
int Light_getMinThreshold(void);

// Set the maximum safety threshold for light level
void Light_setMaxThreshold(int max);

// Get the maximum safety threshold for light level
int Light_getMaxThreshold(void);

// Returns whether the light level is healthy for the plant or not
// 0  : Moderate
// 1  : Excessive
// -1 : Deficient
int Light_getStatus(void);

#endif 
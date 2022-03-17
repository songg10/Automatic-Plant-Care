// Header defining functions to read the value of the moisture sensor.

#ifndef _MOIST_SENSOR_H_
#define _MOIST_SENSOR_H_

// Read the data from the sensor
void Moisture_readSensor(void);

// Return the moisture level
int Moisture_getMoistLevel(void);

// Return the temperature in Celsius
float Moisture_getTemp(void);

// Set the minimum safety threshold for moisture level
void Moisture_setMinThreshold(int min);

// Get the minimum safety threshold for moisture level
int Moisture_getMinThreshold(void);

// Set the maximum safety threshold for moisture level
void Moisture_setMaxThreshold(int max);

// Get the maximum safety threshold for moisture level
int Moisture_getMaxThreshold(void);

// Returns whether the moisture level is healthy for the plant or not
// 0  : Moderate
// 1  : Excessive
// -1 : Deficient
int Moisture_getStatus(void);

#endif 
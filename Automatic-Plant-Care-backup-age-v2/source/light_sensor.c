#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "light_sensor.h"
#include "utils.h"

#define A2D_FILE_VALUE      "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"
#define A2D_VOLTAGE_REF_V   1.8
#define MAX_RAW             4095    
#define LOG_RANGE           5       // 3.3V = 10^5 lux

#define MIN_LUX             0       // 0 lux
#define MAX_LUX             130000  // 130,000 lux

// Store the safety threshold for the plant's light level
static unsigned int MIN_LIGHT = MIN_LUX;      
static unsigned int MAX_LIGHT = MAX_LUX; 

/* Function to calculate x raised to the power y in O(logn)
    Time Complexity of optimized solution: O(logn)
*/
// Adopted from: https://stackoverflow.com/a/26860684
int power2 (int x, unsigned int y)
{
    int temp;
    if (y == 0)
        return 1;

    temp = power2 (x, y / 2);
    if ((y % 2) == 0)
        return temp * temp;
    else
        return x * temp * temp;
}

// Convert the raw reading into lumens (unit for measuring light)
int Light_getLumens () {
    // Get the raw reading
    int raw = Utils_readFromFileToScreen(A2D_FILE_VALUE);

    // Convert to lumens 
    float logLux = raw * LOG_RANGE / MAX_RAW;

    return power2(10, ceil(logLux));
}

// Set the minimum safety threshold for light level
void Light_setMinThreshold(int min) {
    if (min < MIN_LUX || min > MAX_LIGHT)
        return;
    MIN_LIGHT = min;
}

// Get the minimum safety threshold for light level
int Light_getMinThreshold() {
    return MIN_LIGHT;
}

// Set the maximum safety threshold for light level
void Light_setMaxThreshold(int max) {
    if (max > MAX_LUX || max < MIN_LIGHT)
        return;
    MAX_LIGHT = max;
}

// Get the maximum safety threshold for light level
int Light_getMaxThreshold() {
    return MAX_LIGHT;
}

// Returns whether the light level is healthy for the plant or not
// 0  : Moderate
// 1  : Excessive
// -1 : Deficient
int Light_getStatus() {
    int light_level = -1;
    unsigned int vol = (unsigned int)Light_getLumens();

    if (vol > MAX_LIGHT)
        light_level = 1;
    else if (vol >= MIN_LIGHT && vol <= MAX_LIGHT)
        light_level = 0;
    else 
        light_level = -1;

    return light_level;
}

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "light_sensor.h"
#include "utils.h"

#define A2D_FILE_VOLTAGE    "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"
#define A2D_VOLTAGE_REF_V   3.3
#define A2D_MAX_READING     4095
#define SQR(x)              x*x
#define MIN                 0
#define MAX                 4095

// Store the safety threshold for the plant's light level
static unsigned int MIN_LIGHT = 2400;
static unsigned int MAX_LIGHT = 2400;

// meaning of 'precision': the returned answer should be base^x, where
//                         x is in [power-precision/2,power+precision/2]
static double mypow( double base, double power, double precision ) {   
   if ( power < 0 ) return 1 / mypow( base, -power, precision );
   if ( power >= 10 ) return SQR( mypow( base, power/2, precision/2 ) );
   if ( power >= 1 ) return base * mypow( base, power-1, precision );
   if ( precision >= 1 ) return sqrt( base );
   return sqrt( mypow( base, power*2, precision*2 ) );
}

// Returns the raw sensor voltage reading
int Light_getRawVoltage() {
    return Utils_readFromFileToScreen(A2D_FILE_VOLTAGE);
}

// Convert the voltage reading into lumens (unit for measuring light)
double Light_getLumens () {
    // Get the voltage reading
    int voltage = Utils_readFromFileToScreen(A2D_FILE_VOLTAGE);

    // Convert to lumens 
    double lux = ((double)voltage / A2D_MAX_READING) * A2D_VOLTAGE_REF_V;

    return 27.565 * mypow(10, lux, .000001);
}

// Set the minimum safety threshold for light level
void Light_setMinThreshold(int min) {
    if (min < MIN || min > MAX_LIGHT)
        return;
    MIN_LIGHT = min;
}

// Get the minimum safety threshold for light level
int Light_getMinThreshold() {
    return MIN_LIGHT;
}

// Set the maximum safety threshold for light level
void Light_setMaxThreshold(int max) {
    if (max > MAX || max < MIN_LIGHT)
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
    double vol = Light_getRawVoltage();

    if (vol > MAX_LIGHT)
        light_level = 1;
    else if (vol >= MIN_LIGHT && vol <= MAX_LIGHT)
        light_level = 0;
    else 
        light_level = -1;

    return light_level;
}

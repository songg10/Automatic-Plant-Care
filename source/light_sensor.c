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

#define MIN_LUX             0       // 0 lux
#define MAX_LUX             130000  // 130,000 lux

#define SQR(x)              x*x

// Store the safety threshold for the plant's light level
static unsigned int MIN_LIGHT = MIN_LUX;      
static unsigned int MAX_LIGHT = MAX_LUX; 

// meaning of 'precision': the returned answer should be base^x, where
//                         x is in [power-precision/2,power+precision/2]
static double mypow( double base, double power, double precision ) {   
   if ( power < 0 ) return 1 / mypow( base, -power, precision );
   if ( power >= 10 ) return SQR( mypow( base, power/2, precision/2 ) );
   if ( power >= 1 ) return base * mypow( base, power-1, precision );
   if ( precision >= 1 ) return sqrt( base );
   return sqrt( mypow( base, power*2, precision*2 ) );
}

// Convert the raw reading into lumens (unit for measuring light)
double Light_getLumens () {
    // Get the raw reading
    int raw = Utils_readFromFileToScreen(A2D_FILE_VALUE);

    // Convert to lumens 
    double lux = ((double)raw / MAX_RAW) * A2D_VOLTAGE_REF_V;

    return 27.565 * mypow(10, lux, .000001);
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

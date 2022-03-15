#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "light_sensor.h"
#include "utils.h"

#define A2D_FILE_VOLTAGE    "/sys/bus/iio/devices/iio:device6/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V   3.3
#define A2D_MAX_READING     4095
#define SQR(x)              x*x

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

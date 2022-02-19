#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define A2D_FILE_VOLTAGE "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 3.3
#define A2D_MAX_READING 4095

double sqr( double x ) { return x * x; }
// meaning of 'precision': the returned answer should be base^x, where
//                         x is in [power-precision/2,power+precision/2]
double mypow( double base, double power, double precision )
{   
   if ( power < 0 ) return 1 / mypow( base, -power, precision );
   if ( power >= 10 ) return sqr( mypow( base, power/2, precision/2 ) );
   if ( power >= 1 ) return base * mypow( base, power-1, precision );
   if ( precision >= 1 ) return sqrt( base );
   return sqrt( mypow( base, power*2, precision*2 ) );
}

int getVoltageReading()
{
    // Open file
    FILE *f = fopen(A2D_FILE_VOLTAGE, "r");
    if (!f)
    {
        printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
        printf(" Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }
    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0)
    {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);


    return a2dReading;
}

double voltageToLumen (int voltage) {
    double lux = ((double)voltage / A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
    return 27.565 * mypow(10, lux, .000001);
}


int main () {
    while (1) {
        printf("lumens currently is:  %f\n", voltageToLumen(getVoltageReading()));
        printf("voltag currently is:  %f\n", ((double)getVoltageReading() / A2D_MAX_READING) * 5);
        sleep(1);
    }
    return 0;
}

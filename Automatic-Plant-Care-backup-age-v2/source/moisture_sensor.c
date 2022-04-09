#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "moisture_sensor.h"

#define RAW_MIN 200
#define RAW_MAX 2000

#define MIN_VMC 0
#define MAX_VMC 100

// Store the safety threshold for the plant's moisture level
static int MIN_MOIST = MIN_VMC;
static int MAX_MOIST = MAX_VMC;

static int moist_raw = 200;
static float moist_temp = 20.0;

// Read the data from the sensor
void Moisture_readSensor(void) {
    // Read data from moisture sensor using Adafruit's Python library
    system("python3 scripts/moisture_sensor.py");

    // Read the data read from Python into C
    FILE* ptr;
    {
        // Opening file in reading mode
        ptr = fopen("moisture.dat", "r");
    
        if (NULL == ptr) {
            printf("Can't read moist data\n");
        }
    
        // Reading moisture raw data
        fscanf(ptr,"%d", &moist_raw);

        // Reading moisture temperature
        fscanf(ptr,"%f", &moist_temp);
    }

    // Closing the file
    fclose(ptr);

    return;
}

// Return the raw moisture level
// Adopted from: https://github.com/adafruit/Adafruit_CircuitPython_seesaw/blob/fd26309627c0758c982d272b6c708322102bddbf/adafruit_seesaw/seesaw.py#L272
int Moisture_getMoistLevel(void) {
    return moist_raw;
}

// Return the temperature in Celsius
// Adopted from: https://github.com/adafruit/Adafruit_CircuitPython_seesaw/blob/fd26309627c0758c982d272b6c708322102bddbf/adafruit_seesaw/seesaw.py#L357
float Moisture_getTemp(void) {
    return moist_temp;
}

// Return the VMC percantage
int Moisture_getVMC(void) {
    int vmc = (float)moist_raw/(float)RAW_MAX*100;
    return vmc;
}

// Set the minimum safety threshold for moisture level
void Moisture_setMinThreshold(int min) {
    if (min < MIN_VMC || min > MAX_MOIST)
        return;
    MIN_MOIST = min;
}

// Get the minimum safety threshold for moisture level
int Moisture_getMinThreshold(void) {
    return MIN_MOIST;
}

// Set the maximum safety threshold for moisture level
void Moisture_setMaxThreshold(int max) {
    if (max > MAX_VMC || max < MIN_MOIST) 
        return;
    MAX_MOIST = max;
}

// Get the maximum safety threshold for moisture level
int Moisture_getMaxThreshold() {
    return MAX_MOIST;
}

// Returns whether the moisture level is healthy for the plant or not
// 0  : Moderate
// 1  : Excessive
// -1 : Deficient
int Moisture_getStatus(void) {
    int moisture_level = Moisture_getVMC();
    if (moisture_level > MAX_MOIST)
        return 1;
    else if (moisture_level >= MIN_MOIST && moisture_level <= MAX_MOIST)
        return 0;

    return -1;
}   

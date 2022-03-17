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

#define MIN 0
#define MAX 4095

// Store the safety threshold for the plant's moisture level
static unsigned int MIN_MOIST = 2400;
static unsigned int MAX_MOIST = 2400;
static int moist_raw;
static float moist_temp;

// Read the data from the sensor
void Moisture_readSensor(void) {
    // Read data from moisture sensor using Adafruit's Python library
    system("python3 scripts/moisture_sensor.py > /dev/null");

    // Read the data read from Python into C
    FILE* ptr;
    {
        // Opening file in reading mode
        ptr = fopen("scripts/moisture.data", "r");
    
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

// Set the minimum safety threshold for moisture level
void Moisture_setMinThreshold(int min) {
    if (min < MIN || min > MAX_MOIST)
        return;
    MIN_MOIST = min;
}

// Get the minimum safety threshold for moisture level
int Moisture_getMinThreshold(void) {
    return MIN_MOIST;
}

// Set the maximum safety threshold for moisture level
void Moisture_setMaxThreshold(int max) {
    if (max > MAX || max < MIN_MOIST) 
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
    int moisture_level = Moisture_getMoistLevel();
    if (moisture_level > MAX_MOIST)
        moisture_level = 1;
    else if (moisture_level >= MIN_MOIST && moisture_level <= MAX_MOIST)
        moisture_level = 0;
    else 
        moisture_level = -1;

    return 1;
}   

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

// User-defined header
#include "health_monitor.h"
#include "light_sensor.h"
#include "pump_control.h"

// Private variables
static bool stop;
static pthread_t monitor_t;

// Monitoring the plant's moisture level
// Check the moisture level every 5 minutes
// If the moisture level is too low then start
// pumping water
static void *startMonitoring(void *input) {
    while (!stop) {
        // Get the moisture level
        // If the moisture level is below critical level
        // Start the pump for 1.5 second
        int moisture_level = Moisture_getMoistLevel();
        if (moisture_level < 580) {
            PC_timePump(1, 500000000);
        }

        // Check once every 5 minutes
        sleep(300);
    }

    return NULL;
}

// Start a new thread to start monitoring plant's health
void Health_initMonitor() {
    stop = false;   

    // Start the thread
    // Shutdown the program if the thread could not be created
    int status = pthread_create(&monitor_t, NULL, &startMonitoring, NULL);
    if (status) {
        fprintf(stdout, "Cannot start the health monitoring thread!\n");
        exit(-1);
    }
}   

// Stop the monitoring thread
void Health_stopMonitor() {
    stop = true;

    int status = pthread_join(monitor_t, NULL);
    if (status) {
        fprintf(stdout, "Unable to join health monitoring thread!\n");
        exit(-1);
    }
}

// Returns whether the light level is healthy for the plant or not
// 0 : Medium light
// 1 : High light level
// -1: Low light level
int Health_getLightHealth() {
    int light_level = -1;
    double vol = Light_getRawVoltage();

    if (vol > 2000)
        light_level = 1;
    else if (vol >= 1000 && vol <= 2000)
        light_level = 0;
    else 
        light_level = -1;

    return light_level;
}

// Returns whether the moisture level is healthy for the plant or not
// 0: Not healthy
// 1: Healthy
bool Health_getMoistureHealth(void) {
    int moisture_level = Moisture_getMoistLevel();
    if (moisture_level < 580)
        return 0;

    return 1;
}   
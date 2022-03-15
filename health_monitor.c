#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// User-defined header
#include "health_monitor.h"
#include "light_sensor.h"
#include "pump_control.h"
#include "moisture_sensor.h"

#define MAX_EMAIL_SIZE 50

// Private variables
static bool stop;
static pthread_t monitor_t;
char email[MAX_EMAIL_SIZE];

static void send_email_noti(int moist, int light) {
    // Call the python script to generate the email to the user
    execlp("python3", "python3", "plant_email.py", "-e", email, "-m", moist, "-l", light, NULL);
}

// Monitoring the plant's moisture level
// Check the moisture level every 5 minutes
// If the moisture level is too low then start
// pumping water
static void *startMonitoring(void *input) {
    while (!stop) {
        // Get the moisture level
        // If the moisture level is below critical level
        // Start the pump for 1.5 second
        bool moisture_health = Health_getMoistureHealth();
        if (moisture_health == 0) {
            PC_timePump(1, 500000000);
        }

        int light_health = Health_getLightHealth();

        // If either of a condition is below critical level
        // Generate the email notifying the user
        if (moisture_health == 0 || light_health == 0)
            send_email_noti(moisture_health, light_health);

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

// Configure the email for notifications
void Health_setUserEmail(char *user_email) {
    strncpy(email, user_email, MAX_EMAIL_SIZE);
}
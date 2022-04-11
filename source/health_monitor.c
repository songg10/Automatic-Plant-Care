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
#include "watchdog.h"

// Maximum email name length is 320 characters + 1 null character
#define MAX_EMAIL_SIZE 321

// The size of the string that will be used to call the emailing script
// minus the email size
#define MAX_COMMAND_SIZE 47

// Threading variables
static bool stop;
static pthread_t monitor_t;

// Store the email of the user
static char email[MAX_EMAIL_SIZE];

static void send_email_noti(int moist, int light, bool isSample) {
    // Call the python script to generate the email to the user
    char command[MAX_EMAIL_SIZE+MAX_COMMAND_SIZE];
    if (isSample) {
        snprintf(command, MAX_EMAIL_SIZE+MAX_COMMAND_SIZE, "python3 scripts/plant_email.py -e %s -s %d", email, isSample);
    } else {
        snprintf(command, MAX_EMAIL_SIZE+MAX_COMMAND_SIZE, "python3 scripts/plant_email.py -e %s -m %d -l %d", email, moist, light);
    }
    // printf("%s\n", command);
    system(command);
}

// Monitoring the plant's moisture level
// Check the moisture level every 5 minutes
// If the moisture level is too low then start
// pumping water
static void *startMonitoring(void *input) {
    while (!stop) {
        // Get the moisture and light level
        Moisture_readSensor();
        int moisture_health = Moisture_getStatus();
        int light_health = Light_getStatus();

        // If either of a condition is below critical level
        // Generate the email notifying the user
        if (moisture_health != 0 || light_health != 0){
            // If the moisture level is below critical level
            // Start the pump for 1.5 second
            if (moisture_health < 0) {
                PC_timePump(1, 500000000);
            }

            send_email_noti(moisture_health, light_health, false);
        }

        // Hit the watchdog
        // WD_hitWatchdog();
        
        // Check once every 15 seconds
        sleep(15);
    }

    return NULL;
}

// Start a new thread to start monitoring plant's health
void Health_initMonitor() {
    stop = false;   

    strncpy(email, "sample_email@gmail.com", MAX_EMAIL_SIZE);

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

// get the user email
char *Health_getUserEmail() {
    return email;
}

// Configure the email for notifications
void Health_setUserEmail(char *user_email) {
    strncpy(email, user_email, MAX_EMAIL_SIZE);
}

// Send a sample email to the user
void Health_sendSampleEmail() {
    send_email_noti(0, 0, true);
}
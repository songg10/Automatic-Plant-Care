// Header defining functions for health monitoring module

#ifndef _HEALTH_MONITOR_H
#define _HEALTH_MONITOR_H

#include <stdbool.h>

// Start a new thread to start monitoring plant's health
void Health_initMonitor(void);

// Stop the monitoring thread
void Health_stopMonitor(void);

// Get the user email
char *Health_getUserEmail(void);

// Configure the email for notifications
void Health_setUserEmail(char *email);

// Send a sample email to the user
void Health_sendSampleEmail(void);

#endif 
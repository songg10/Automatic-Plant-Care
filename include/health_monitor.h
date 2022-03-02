// Header defining functions for health monitoring module

#ifndef _HEALTH_MONITOR_H
#define _HEALTH_MONITOR_H

#include <stdbool.h>

// Start a new thread to start monitoring plant's health
void Health_initMonitor(void);

// Stop the monitoring thread
void Health_stopMonitor(void);

// Returns whether the light level is healthy for the plant or not
// 0 : Medium light
// 1 : High light level
// -1: Low light level
int Health_getLightHealth(void);

// Returns whether the moisture level is healthy for the plant or not
// 0: Not healthy
// 1: Healthy
bool Health_getMoistureHealth(void);

#endif 
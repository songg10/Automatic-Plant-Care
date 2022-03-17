// Header defining functions to control the water pump.

#ifndef _PUMP_CONTROL_H_
#define _PUMP_CONTROL_H_

#include <stdbool.h>

// Initialize the GPIO for controlling the pump (must be called before anything else)
void PC_initPump(void);

// Clean up the data
void PC_cleanUp();

// Start pumping water 
void PC_startPump(void);

// Stop pumping water
void PC_stopPump(void);

// Pump water for the duration specified
void PC_timePump(long sec, long nanosec);

// Get the status of the pump (on or off)
bool PC_getStatus();

#endif
// Header defining functions to control the water pump.

#ifndef _PUMP_CONTROL_H_
#define _PUMP_CONTROL_H_

// Initialize the GPIO for controlling the pump (must be called before anything else)
void PC_initPump(void);

// Start pumping water 
void PC_startPump(void);

// Stop pumping water
void PC_stopPump(void);

// Pump water for the duration specified
void PC_timePump(long sec, long nanosec);

#endif
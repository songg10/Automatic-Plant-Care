#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "pump_control.h"

// Place holder GPIO ports as the hardware has not
// been physically connected
#define GPIO "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_NUM "20"
#define GPIO_DIR "/sys/class/gpio/gpio20/direction"
#define GPIO_VAL "/sys/class/gpio/gpio20/value"

static bool isOn;

static void nsleep(long sec, long nanosec) {
    long seconds = sec;
    long nanoseconds = nanosec;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *)NULL);
}

// Initialize the GPIO for controlling the pump (must be called before anything else)
void PC_initPump() {
    // Export the GPIO pins
    Utils_writeToFile(GPIO, GPIO_NUM);

    nsleep(0, 300000000);

    // Set up the GPIO pin as output
    Utils_writeToFile(GPIO_DIR, "out");

    isOn = false;
}

// Clean up the data
void PC_cleanUp() {
    PC_stopPump();
    isOn = false;
    Utils_writeToFile(GPIO_UNEXPORT, GPIO_NUM);
}

// Start pumping water
void PC_startPump() {
    Utils_writeToFile(GPIO_VAL, "1");

    isOn = true;
}

// Stop pumping water
void PC_stopPump() {
    Utils_writeToFile(GPIO_VAL, "0");

    isOn = false;
}

// Toggle the pump
void PC_togglePump(void) {
    if (isOn) {
        PC_stopPump();
    }   
    else {
        PC_startPump();
    }
}

// Pump water for the duration specified
void PC_timePump(long sec, long nanosec) {
    PC_startPump();
    nsleep(sec, nanosec);
    PC_stopPump();
}

// Get the status of the pump (on or off)
bool PC_getStatus() {
    return isOn;
}

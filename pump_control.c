#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

// Place holder GPIO ports as the hardware has not 
// been physically connected
#define GPIO     "/sys/class/gpio/export"
#define GPIO_NUM "30"
#define GPIO_DIR "/sys/class/gpio/gpio30/direction"
#define GPIO_VAL "/sys/class/gpio/gpio30/value"

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

  // Set up the GPIO pin as output
  Utils_writeToFile(GPIO_DIR, "out");
}

// Start pumping water 
void PC_startPump() {
  Utils_writeToFile(GPIO_VAL, "1");
}

void PC_stopPump() {
  Utils_writeToFile(GPIO_VAL, "0");
}

void PC_timePump(long sec, long nanosec) {
  PC_startPump();
  nsleep(sec, nanosec);
  PC_stopPump();
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>      /* needed to use pid_t, etc. */
#include <sys/wait.h>       /* needed to use wait() */  

#include "pump_control.h"
#include "health_monitor.h"
#include "udpServer.h"
#include "light_sensor.h"
#include "moisture_sensor.h"
#include "lcd.h"

// Launch the GUI on the touch screen
static inline pid_t GUI_init() {
    // Tell the system to launch the GUI on the attached screen
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error forking the GUI process\n");
        exit(-1);
    } 

    if (pid == 0) {
        // Child proces
        execlp("./plant_gui", "./plant_gui", NULL);
        exit(-1);
    } 
    return pid;
}

int main() {
    // Initialize the back-end modules 
    PC_initPump();
    Health_initMonitor();
    LCD_init();
    UDP_initServer();

    // Init the GUI
    pid_t child_pid = GUI_init();

    // Clean-up the back-end process
    UDP_stopServer();
    LCD_cleanUp();
    Health_stopMonitor();
    PC_cleanUp();

    kill(child_pid, SIGKILL);
    wait(NULL);
    printf("Process ID = %d has been terminated\n", child_pid);

    return 0;
}
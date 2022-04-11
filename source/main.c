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
#include "backup.h"

// Launch the GUI on the touch screen
static inline void GUI_init() {
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
}

int main() {

    // How I suggest starting the program to include backup:
    // Check if the backup file exists (ie user has already set up the plant)
    // If it exists, simply reload the saved values
    // Otherwise, set the plant's DOB to today's date. 
    //
    // if (access(BACKUP_FILE, F_OK) == 0) {
    //     restore_info();
    // } 
    // else {
    //     Age_reset_dob();
    // }


    // Initialize the back-end modules && the GUI
    PC_initPump();
    Health_initMonitor();
    // LCD_init();
    UDP_initServer();
    // GUI_init();

    // Clean-up the back-end process
    UDP_stopServer();
    // LCD_cleanUp();
    Health_stopMonitor();
    PC_cleanUp();
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pump_control.h"
#include "health_monitor.h"
#include "udpServer.h"
#include "light_sensor.h"
#include "moisture_sensor.h"
#include "backup.h"

int main() {
    // How I suggest starting the program:
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
    PC_initPump();
    Health_initMonitor();
    UDP_initServer();

    UDP_stopServer();
    Health_stopMonitor();
    PC_cleanUp();
    
    return 0;
}
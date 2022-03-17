#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pump_control.h"
#include "health_monitor.h"
#include "udpServer.h"
#include "light_sensor.h"
#include "moisture_sensor.h"

int main() {
    PC_initPump();
    Health_initMonitor();
    UDP_initServer();

    UDP_stopServer();
    Health_stopMonitor();
    PC_cleanUp();
    
    return 0;
}
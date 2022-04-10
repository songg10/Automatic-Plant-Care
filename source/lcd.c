#include <stdio.h>
#include <poll.h> 
#include "lcd.h"

// Preprocessor macro to avoid compiling this portion for the GUI
// #ifndef GUI
#include <pthread.h>
#include "pump_control.h"
#include "moisture_sensor.h"
#include "light_sensor.h"
#include "pump_control.h"

// Threading variables
static bool stop;
static pthread_t lcd_t;

static void *startLCDHandler(void *input) {
    // Open the request FIFO
    int req_fifo = open(LCD_REQUEST,O_RDONLY|O_NONBLOCK);
    if (req_fifo == -1) {
		printf("lcd: Unable to access request FIFO\n");
	}

    // Mark the LCD response fifo as unopen (the read end must be opened first)
    int res_fifo = UNOPEN;

    // Start servicing the GUI
    while (!stop) {
        // Read the request from the GUI
        LCD_Request req = {0};
        ssize_t bytes_read = read(req_fifo,&req,sizeof(req));
        if (bytes_read > 0) {
            if (req.togglePump) {
                PC_togglePump();
            }
        }

        // Collecting data to send back to the GUI
        int vmc = Moisture_getVMC();
        float temperature = Moisture_getTemp();
        int lux = Light_getLumens();
        bool pumpOn = PC_getStatus();
        LCD_Response res = {vmc, lux, temperature, pumpOn};
        
        // Sending back the data
        if (res_fifo < 0) {
            res_fifo = open(LCD_RESPONSE,O_WRONLY|O_NONBLOCK);
            if (res_fifo < 0) {
                printf("lcd: Unable to access response FIFO\n");
            } 
        }
        write(res_fifo, &res, sizeof(res));
        sleep(1);
    }

    close(res_fifo);
    close(req_fifo);

    return NULL;
}

// Initialize LCD hanlder
void LCD_init() {
    stop = false;   

    // Unlink the old FIFOs
    unlink(LCD_REQUEST);
    unlink(LCD_RESPONSE);

    // Make the FIFO
    int file = mkfifo(LCD_REQUEST, 0666); 
    if (file < 0) {
        printf("Unable to create the FIFO\n");
        exit(-1);
    }

    // Make the FIFO
    file = mkfifo(LCD_RESPONSE, 0666); 
    if (file < 0) {
        printf("Unable to create the FIFO\n");
        exit(-1);
    }

    // Start the thread
    // Shutdown the program if the thread could not be created
    int status = pthread_create(&lcd_t, NULL, &startLCDHandler, NULL);
    if (status) {
        fprintf(stdout, "Cannot start the LCD handler thread!\n");
        exit(-1);
    }
}

// Clean up the LCD handler
void LCD_cleanUp() {
    stop = true;

    int status = pthread_join(lcd_t, NULL);
    if (status) {
        fprintf(stdout, "Unable to join LCD handler thread!\n");
        exit(-1);
    }
}

#include <stdio.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "lcd.h"

// Preprocessor macro to avoid compiling this portion for the GUI
#ifndef GUI
#include <pthread.h>
#include "pump_control.h"
#include "moisture_sensor.h"
#include "light_sensor.h"
#include "pump_control.h"

// Threading variables
static bool stop;
static pthread_t lcd_t;

static void *startLCDHandler(void *input) {
    while (!stop) {
        // Read the soil moisture sensor reading
        // Moisture_readSensor();
        int vmc = Moisture_getVMC();
        float temperature = Moisture_getTemp();

        // Read the ambient light sensor reading
        int lux = Light_getLumens();

        // Get the pump status
        bool pumpOn = PC_getStatus();

        // Send the data to the LCD screen
        LCD_Response res = {vmc, lux, temperature, pumpOn};
        LCD_generateResponse(&res);

        // Process pump request from LCD
        LCD_Request req = {0};
        ssize_t bytes = LCD_getRequest(&req);
        if (bytes != 0 && bytes != -1) {
            if (req.togglePump) {
                PC_togglePump();
            } 
        }

        // Sleep for 1s
        sleep(1);
    }

    return NULL;
}

// Initialize LCD hanlder
void LCD_init() {
    stop = false;   

    // Make the FIFO
    int file = mkfifo(LCD_REQUEST, 0666); 
    if (file < 0) {
        printf("Unable to create the FIFO");
        exit(-1);
    }

    // Make the FIFO
    file = mkfifo(LCD_RESPONSE, 0666); 
    if (file < 0) {
        printf("Unable to create the FIFO");
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

#endif

// ----------------------------------------------------------------------------------------------------------------------------------------------

// Write the data to the FIFO
void LCD_generateResponse(LCD_Response *res) {
    // Open the named pipe (FIFO)
    int lcd_fifo = open(LCD_RESPONSE,O_WRONLY|O_NONBLOCK);
    if(lcd_fifo < 0) {
        printf("Error in opening %s\n", LCD_RESPONSE);
    }

    // Transfer data to LCD
    ssize_t bytes_written = write(lcd_fifo,res,sizeof(LCD_Response));
    if (bytes_written != sizeof(LCD_Response)) {
        printf("Unable to write the whole data to the pipe!\n");
    }

    // Close the named pipe (FIFO)
    close(lcd_fifo);
}

void LCD_generateRequest(LCD_Request *req) {
    // Open the named pipe (FIFO)
    int lcd_fifo = open(LCD_REQUEST,O_WRONLY|O_NONBLOCK);
    if(lcd_fifo < 0) {
        printf("Error in opening %s\n", LCD_REQUEST);
    }

    // Transfer data to LCD
    ssize_t bytes_written = write(lcd_fifo,req,sizeof(LCD_Request));
    if (bytes_written != sizeof(LCD_Request)) {
        printf("Unable to write the whole data to the pipe!\n");
    }

    // Close the named pipe (FIFO)
    close(lcd_fifo);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------

// Read the request from the FIFO
ssize_t LCD_getResponse(LCD_Response *res) {
    // Open the named pipe (FIFO)
    int lcd_fifo = open(LCD_RESPONSE,O_RDONLY|O_NONBLOCK);
    if(lcd_fifo < 0) {
        printf("Error in opening %s\n", LCD_RESPONSE);
    }

    // Transfer data to LCD
    ssize_t bytes_read = read(lcd_fifo,res,sizeof(LCD_Response));
    if (bytes_read != -1) {
        printf("Unable to read the data from FIFO!\n");
    }

    // Close the named pipe (FIFO)
    close(lcd_fifo);

    return bytes_read;
}

ssize_t LCD_getRequest(LCD_Request *req) {
    // Open the named pipe (FIFO)
    int lcd_fifo = open(LCD_REQUEST,O_RDONLY|O_NONBLOCK);
    if(lcd_fifo < 0) {
        printf("Error in opening %s\n", LCD_REQUEST);
    }

    // Transfer data to LCD
    ssize_t bytes_read = read(lcd_fifo,req,sizeof(LCD_Request));
    if (bytes_read != -1) {
        printf("Unable to read the data from FIFO!\n");
    }

    // Close the named pipe (FIFO)
    close(lcd_fifo);

    return bytes_read;
}
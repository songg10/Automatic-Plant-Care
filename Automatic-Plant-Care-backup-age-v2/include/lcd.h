#ifndef _GUI_DATA_
#define _GUI_DATA

#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

#define LCD_REQUEST  "lcd_request"
#define LCD_RESPONSE "lcd_response"

typedef struct {
    int vmc;                // in VMC percentage
    int lux;              // in Lumens
    float temperature;      // in Celsius
    bool pumpOn;            // Indicate whether the pump is on or not
} LCD_Response;

typedef struct {
    bool togglePump;        // Toggle the pump
} LCD_Request;

// Initialize LCD hanlder
void LCD_init();

// Clean up the LCD handler
void LCD_cleanUp();

// Write the data to the FIFO
void LCD_generateResponse(LCD_Response *res);
void LCD_generateRequest(LCD_Request *req);

// Read the request from the FIFO
ssize_t LCD_getResponse(LCD_Response *res);
ssize_t LCD_getRequest(LCD_Request *req);

#endif
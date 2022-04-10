#ifndef _GUI_DATA_
#define _GUI_DATA

#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LCD_REQUEST     "tmp/lcd_request"
#define LCD_RESPONSE    "tmp/lcd_response"

#define UNOPEN -1

typedef struct {
    int vmc;                // in VMC percentage
    int lux;              // in Lumens
    float temperature;      // in Celsius
    bool pumpOn;            // Indicate whether the pump is on or not
} LCD_Response;

typedef struct {
    bool togglePump;        // Toggle the pump
} LCD_Request;

#ifndef GUI

// Initialize LCD hanlder
void LCD_init();

// Clean up the LCD handler
void LCD_cleanUp();

#endif

#endif
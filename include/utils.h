// Header defining utility function prototypes for read from and write to files.

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <stdio.h>
#include <stdlib.h>
#include "backup.h"

// Utility functions to read from or write to file
int Utils_readFromFileToScreen(const char *fileName);
void Utils_writeToFile(const char *fileName, const char *command);
int Utils_read_bin(const char* fileName, char* buffer);
int Utils_write_bin(const char* fileName, const char* value, int size);

#endif

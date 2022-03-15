// Header defining utility function prototypes for read from and write to files.

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <stdio.h>
#include <stdlib.h>

// Utility functions to read from or write to file
int Utils_readFromFileToScreen(const char *fileName);
void Utils_writeToFile(const char *fileName, const char *command);

#endif
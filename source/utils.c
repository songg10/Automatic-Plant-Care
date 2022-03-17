// Source code implementing utility functions for read from and write to LED and GPIO files.

#include "utils.h"

int Utils_readFromFileToScreen(const char *fileName) {
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }

    // Read string (line)
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);

    // Close
    fclose(pFile);
    
    return atoi(buff);
}

void Utils_writeToFile(const char *fileName, const char *command) {
    FILE *pFile = fopen(fileName, "w");
    if (pFile == NULL) {
        printf("ERROR OPENING %s.\n", fileName);
        exit(1);
    }

    int charWritten = fprintf(pFile, "%s", command);
    if (charWritten <= 0) {
        printf("ERROR WRITING DATA to %s\n", fileName);
        exit(1);
    }

    fclose(pFile);
}
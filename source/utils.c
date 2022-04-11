// Source code implementing utility functions for read from and write to LED and GPIO files.

#include "utils.h"

#define MAX_LENGTH 1024

int Utils_readFromFileToScreen(const char *fileName) {
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }

    // Read string (line)
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

int Utils_read_bin(const char* fileName, char* buffer)
{
    FILE *read_ptr;
    read_ptr = fopen(fileName, "r");
    if (read_ptr == NULL){
        return -1;
    }
    int i = fread(buffer, MAX_LENGTH, 1, read_ptr);
    buffer[i] = '\0';
    fclose(read_ptr);
    return 0;
}

int Utils_write_bin(const char* fileName, const char* value, int size)
{
    FILE *write_ptr;
    write_ptr = fopen(fileName, "w");
    if (write_ptr == NULL){
        return -1;
    }
    if (fwrite(value, size, 1, write_ptr) != size){
        fclose(write_ptr);
        return -1;
    }
    fclose(write_ptr);
    return 0;
}
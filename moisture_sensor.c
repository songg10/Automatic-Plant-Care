#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "moisture_sensor.h"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"

#define I2C_DEVICE_ADDRESS 0x36

#define TOUCH_BASE 0x0F
#define TOUCH_MSB  0x01         
#define TOUCH_LSB  0x00

#define STATUS_BASE 0x00

#define MOIST_REG_COUNT 0x02
#define TEMP_REG_COUNT  0x04

static int initI2cBus(char* bus, int address){
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
    }
    return i2cFileDesc;
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value){
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr, const unsigned int regCount)
{
    // To read a register, must first write the address
    int res;
    for (int i = 0; i < regCount; i++, regAddr++) {
        res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
        if (res != sizeof(regAddr)) {
            perror("Unable to write i2c register.");
            exit(-1);
        }
    }

	// Now read 7 bytes from 7 register
	unsigned char temp[regCount];
	res = read(i2cFileDesc, &temp, sizeof(unsigned char)*regCount);
	if (res != sizeof(temp)) {
		perror("Unable to read i2c register");
		exit(-1);
	}

    // Move the data to the heap so that we can return it
    // This means that the caller would need to free the 
    // memory space themselves
    unsigned char *value = malloc(sizeof(unsigned char)*regCount);
    for (int i = 0; i < regCount; i++) {
        value[i] = temp[i];
    }

	return value;
}

// Return the moisture level
// Adopted from: https://github.com/adafruit/Adafruit_CircuitPython_seesaw/blob/fd26309627c0758c982d272b6c708322102bddbf/adafruit_seesaw/seesaw.py#L272
int Moisture_getMoistLevel(void) {
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    unsigned char *buff = readI2cReg(i2cFileDesc, TOUCH_BASE, MOIST_REG_COUNT);

    int16_t x = (buff[TOUCH_MSB] << 8) | (buff[TOUCH_LSB]);

    free(buff);

    return x;
}

// Return the temperature in Celsius
// Adopted from: https://github.com/adafruit/Adafruit_CircuitPython_seesaw/blob/fd26309627c0758c982d272b6c708322102bddbf/adafruit_seesaw/seesaw.py#L357
unsigned int Moisture_getTemp(void) {
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    unsigned char *buff = readI2cReg(i2cFileDesc, TOUCH_BASE, TEMP_REG_COUNT);

    buff[0] &= 0x3F;
    uint32_t x = 0;
    for (int i = 0; i < MOIST_REG_COUNT; i++) {
        // (buff[0] << 32) | (buff[1] << 16) | (buff[2] << 8) | buff[0];
        x |= buff[i] << ((4-i)*8);
    }

    free(buff);

    return 0.00001525878 * x;
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <Adafruit_BBIO.GPIO.h>
#define GPIO_dir "/sys/class/gpio/gpio30/direction"
#define GPIO_val "/sys/class/gpio/gpio30/value"
//#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"
//#define I2C_DEVICE_ADDRESS 0x3c
//#define RELAY "P9_22"

void writeToFile(char *fileName, char *value) {
  FILE *file = fopen(fileName, "w");
  int charWritten = fprintf(file, "%s", value);
  if (charWritten <= 0)
    printf("ERROR: Write");

  fclose(file);
}

void nsleep(long sec, long nanosec) {
  long seconds = sec;
  long nanoseconds = nanosec;
  struct timespec reqDelay = {seconds, nanoseconds};
  nanosleep(&reqDelay, (struct timespec *)NULL);
}

/*int initI2cBus(char *bus, int address) {
    int fd = open(bus, O_RDWR);
    if(fd < 0) {
        printf("i2c: Unable to open bus for read/write (%s)\n", bus);
        exit(-1);
    }
    int result = ioctl(fd, I2C_SLAVE, address);
    if(result < 0) {
        printf("i2c: Unable to set i2c device to slave address\n");
        exit(-1);
 }
 return fd;
}*/

int main () {
    //start watering for 5s
    writeToFile(GPIO_dir, "out");
    writeToFile(GPIO_val, "0");
    nsleep(5,0);
    //stop watering
    writeToFile(GPIO_dir, "out");
    writeToFile(GPIO_val, "1");

    return 0;
}
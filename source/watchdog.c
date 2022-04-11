// Watchdog example
// by Brian Fraser
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include "watchdog.h"


static int timeout = 0;

void changeWatchdogTimeout(int timeout_s)
{
	//printf("Setting watchdog timout to %ds\n", timeout_s);

	int fd = open("/dev/watchdog", O_RDWR);
	dieOnError(fd != -1, "Unable to open WD.");

	int result = ioctl(fd, WDIOC_SETTIMEOUT, &timeout_s);
	dieOnError(result == 0, "Unable to set watchdog timout.");

	close(fd);
}

void hitWatchdogOnKeypress()
{
	int fd = open("/dev/watchdog", O_RDWR);
	dieOnError(fd != -1, "Unable to open WD.");

    //printf("Hitting WD via ioctl...\n");
    ioctl(fd, WDIOC_KEEPALIVE, NULL);

	// Close file to disable watchdog (or exit program auto-closes)
	close(fd);
}

void dieOnError(_Bool successCondition, char *message)
{
	if (!successCondition) {
		fprintf(stderr, "ERROR: %s\n", message);
		fprintf(stderr, "Error string: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

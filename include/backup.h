// File defining functions necessary to write and write essential 
// program variables to and from a file in memory.

#ifndef _BACKUP_H_
#define _BACKUP_H_

#include <string.h>
#include "health_monitor.h"
#include "light_sensor.h"
#include "moisture_sensor.h"
#include "age.h"
#include "utils.h"

#define NUM_ATTRIBUTES 9
#define BACKUP_FILE "backup.bin"

// Backup/restore the user's email, minimum and maximum thresholds for light and
// moisture level, and plant's DOB: year, month, date_in_month, and day.
// Returns 0 on success, -1 on failure.
int backup_info(void);
int restore_info(void);

#endif
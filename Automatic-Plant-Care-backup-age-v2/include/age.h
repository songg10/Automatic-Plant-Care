// File defining functions to keep track of the age of the plant.

#ifndef _AGE_H_
#define _AGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct date {
    int year;
    int month;
    int day_in_month;
    int day;
};

// Reset the plant's DOB to today's date.
void Age_reset_dob(void);

// Returns a date struct with the plant's age in years and days if age > 1 year,
// otherwise just days. 
// If months are desired as well, divide days by 30. 
struct date Age_get_age(void);

// Sets the plant's DOB to the values in DAY.
void Age_set_dob(struct date day);

// Return's the plant's DOB. 
struct date Age_get_dob(void);


#endif
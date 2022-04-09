
#include "age.h"


static struct date dob;

// struct tm {
//   int tm_sec;         /* seconds,  range 0 to 59          */
//   int tm_min;         /* minutes, range 0 to 59           */
//   int tm_hour;        /* hours, range 0 to 23             */
//   int tm_mday;        /* day of the month, range 1 to 31  */
//   int tm_mon;         /* month, range 0 to 11             */
//   int tm_year;        /* The number of years since 1900   */
//   int tm_wday;        /* day of the week, range 0 to 6    */
//   int tm_yday;        /* day in the year, range 0 to 365  */
//   int tm_isdst;       /* daylight saving time             */	
// };

void Age_reset_dob(void) 
{
    // get today's date
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    // set dob values  to today's values
    dob.year = today.tm_year + 1900;
    dob.month = today.tm_mon + 1;
    dob.day_in_month = today.tm_mday;
    dob.day = today.tm_yday;
}

struct date Age_get_age(void)
{
    // get today's date
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    // calculate the age of the plant in either years and days, 
    // or days if age < 1 year
    struct date age;
    age.year = today.tm_year + 1900 - dob.year;
    age.day = today.tm_yday - dob.day;
    if (age.day < 0){
        age.year -= 1;
        age.day += 365;
    }
    return age;
}

void Age_set_dob(struct date day)
{
    dob = day;
}

struct date Age_get_dob(void)
{
    return dob;
}

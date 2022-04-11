#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

void WD_changeTimeout(int timeout_s);
void WD_hitWatchdog();

#endif
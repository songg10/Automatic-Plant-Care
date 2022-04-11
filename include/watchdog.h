#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

void changeWatchdogTimeout(int timeout_s);
void hitWatchdogOnKeypress();

#endif
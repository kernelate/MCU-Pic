/*
 * File:   watchdog.h
 * Author: ntekcom27
 *
 * Created on December 4, 2015, 2:48 PM
 */

#ifndef WATCHDOG_H
#define	WATCHDOG_H

#ifdef	__cplusplus
extern "C"
{
#endif
    int wtimer = 0;
    int wtimer_prescale = 0;
    int wtimer_flag = 0;

    void cpu_watchdog_handler();
    void auto_on();


#ifdef	__cplusplus
}
#endif

#endif	/* WATCHDOG_H */


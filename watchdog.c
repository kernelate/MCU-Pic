
#include <xc.h>
#include <string.h>
#include "watchdog.h"
#include "debug.h"

#define _XTAL_FREQ 4000000

void cpu_watchdog_handler()
{
    if (wtimer_flag == 1)
    {
        if (wtimer_prescale >= 5)
        { // 100ms
            wtimer_prescale = 0;
            if (wtimer >= 50)
            { // 100ms*50=5secs
                auto_on();
                wtimer = 0;
            } else
            {
                wtimer++;
            }
        } else
        {
            __delay_ms(20);
            wtimer_prescale++;
        }
    }
}
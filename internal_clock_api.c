

#include <xc.h>
#include "internal_clock_Api.h"

void init_internal_clock()
{
    OSCCONbits.IRCF = 0b1101; //4MHZ
    //OSCCONbits.IRCF=0b1110;     //8MHZ
    //OSCCONbits.IRCF=0b1111;     //16MHZ
    OSCCONbits.SCS = 0b11; // for internal clock settings
}


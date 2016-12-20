#define ECHOCANCELLER

#ifdef ECHOCANCELLER
#include <xc.h>
#include "CS6422_Api.h"

#define _XTAL_FREQ 4000000

void echo_cancel_on()
{
    RESET_PIN = 1;
    __delay_ms(2);
}

void echo_cancel_off()
{
    RESET_PIN = 0;
    __delay_ms(2);
}

void strobe_pulse(void)
{
    // switch the STROBE like a clock
    STROBE = 0;
    __delay_ms(STROBE_DELAY_MS);
    STROBE = 1;
    __delay_ms(STROBE_DELAY_MS);
}

void data_ready(int logic)
{
    DRDY = logic;
    __delay_ms(DRDY_DELAY_MS);
}

void cs6422_write_bit(int data_bit)
{
    DATA = data_bit;
    strobe_pulse();
}

int cs6422_write_word(int data_msb, int data_lsb)
{
    int data_bit = 0;
    int ctr = 15;
    int reg_num;


    data_ready(0);

    reg_num = (data_lsb & 0x0f) >> 1;

    ec_msb[reg_num] = data_msb; //save the status of each register accessed
    ec_lsb[reg_num] = data_lsb;
    for (ctr = 7; ctr > -1; ctr--)
    {
        data_bit = (data_msb >> ctr);
        data_bit &= 0x01;
        //DPRINTK("%s[%d]:\t", __func__, ctr);
        cs6422_write_bit(data_bit);
    }
    for (ctr = 7; ctr > -1; ctr--)
    {
        data_bit = (data_lsb >> ctr);
        data_bit &= 0x01;
        //DPRINTK("%s[%d]:\t", __func__, ctr);
        cs6422_write_bit(data_bit);
    }
    data_ready(1);
    //four (4) extra strobe signals
    strobe_pulse();
    strobe_pulse();
    strobe_pulse();
    strobe_pulse();
    return 0;
}

void init_cs6422()
{
    TRIS_DATA = 0;
    TRIS_STROBE = 0;
    TRIS_RESET = 0;
    TRIS_DRDY = 0;
    TRIS_PWDN = 0;
    TRIS_RST = 0;

    DRDY = 1;
    STROBE = 1;
    DATA = 1;
    PWDN = 1;

    echo_cancel_on();
    echo_cancel_off();
    echo_cancel_on();

    //echo_default();
    //echo_write_allreg();

    /*/
    cs6422_write_word(ec_msb[0],ec_lsb[0]);
    cs6422_write_word(ec_msb[1],ec_lsb[1]);
    cs6422_write_word(ec_msb[2],ec_lsb[2]);
    cs6422_write_word(ec_msb[3],ec_lsb[3]);
    cs6422_write_word(ec_msb[4],ec_lsb[4]);
    cs6422_write_word(ec_msb[5],ec_lsb[5]);
    //*/
    echo_cancel_on();
}

#endif

void echo_default()
{
    ec_msb[0] = 0x67;
    ec_lsb[0] = 0x00;
    ec_msb[1] = 0x2d;
    ec_lsb[1] = 0x02;
    ec_msb[2] = 0x00;
    ec_lsb[2] = 0x04;
    ec_msb[3] = 0x00;
    ec_lsb[3] = 0x06;
    ec_msb[4] = 0x00;
    ec_lsb[4] = 0x08;
    ec_msb[5] = 0x00;
    ec_lsb[5] = 0x0a;
}

void echo_write_allreg()
{
    for (int i = 0; i < 6; i++)
    {
        cs6422_write_word(ec_msb[i], ec_lsb[i]);
    }
}
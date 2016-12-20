#include <xc.h>

#define DRDY            PORTDbits.RD4
#define TRIS_DRDY       TRISDbits.TRISD4
#define DATA            PORTDbits.RD7
#define TRIS_DATA       TRISDbits.TRISD7
#define STROBE          PORTDbits.RD6
#define TRIS_STROBE     TRISDbits.TRISD6
#define RESET_PIN       PORTDbits.RD5
#define TRIS_RESET      TRISDbits.TRISD5

#define TRIS_PWDN   TRISCbits.TRISC0
#define PWDN   PORTCbits.RC0
#define TRIS_RST    TRISCbits.TRISC1
#define RST    PORTCbits.RC1

#define STROBE_DELAY_MS     1
#define DRDY_DELAY_MS       1
#define RESET_DELAY_MS      1

void strobe_pulse(void);
void data_ready(int logic);
void cs6422_write_bit(int data_bit);
int cs6422_write_word(int data_msb, int data_lsb);
void init_cs6422();
void echo_cancel_on();
void echo_cancel_off();
void echo_default();
void echo_write_allreg();
int ec_msb[6];
int ec_lsb[6];
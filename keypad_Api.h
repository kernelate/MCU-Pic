/* 
 * File:   keypad_Api.h
 * Author: ntekcom11
 *
 * Created on November 11, 2016, 9:32 AM
 */

#define ROW_ACTIVE       1
#define ROW_INACTIVE     0

#define HIGH    1
#define LOW     0

unsigned char const keyPadMatrix[] =
{
    '1','2','3',
    '4','5','6',
    '7','8','9',
    '*','0','#',
    0xFF
};

unsigned char old_key, keypress;


/*FUNCTIONS*/
void init_keypad();
void GetKeyPressed();
void deinit_keypad();
void ms_delay(unsigned char time);



#include <xc.h>
#include "keypad_Api.h"
#include "i2c_Api.h"
#include "i2c_registers.h"
#include "gpio.h"


void ms_delay(unsigned char time)
{
    unsigned char counter;
    while(time>0)
    {
        counter = 166;
        while(counter>0)
        {
            counter--;
        }
        time--;
    }
}

void init_keypad()
{
    //OUTPUT PIN
    TRIS_COLUMN1 = 0;
    TRIS_COLUMN2 = 0;
    TRIS_COLUMN3 = 0;

    TRIS_ROW1 = 0;
    TRIS_ROW2 = 0;
    TRIS_ROW3 = 0;
    TRIS_ROW4 = 0;

    //DIGITAL PIN
    ANSEL_COLUMN1 = 0;
    ANSEL_COLUMN2 = 0;
    ANSEL_COLUMN3 = 0;

//    ANSEL_ROW1 = 0;
    ANSEL_ROW2 = 0;
    ANSEL_ROW3 = 0;
    ANSEL_ROW4 = 0;

//    LAT_COLUMN1 = LOW;
//    LAT_COLUMN2 = LOW;
//    LAT_COLUMN3 = LOW;
//
//    LAT_ROW1 = HIGH;
//    LAT_ROW2 = HIGH;
//    LAT_ROW3 = HIGH;
//    LAT_ROW4 = HIGH;
}

void deinit_keypad()
{
    //OUTPUT PIN
    TRIS_COLUMN1 = 0;
    TRIS_COLUMN2 = 0;
    TRIS_COLUMN3 = 0;

    TRIS_ROW1 = 1;
    TRIS_ROW2 = 1;
    TRIS_ROW3 = 1;
    TRIS_ROW4 = 1;

    //DIGITAL PIN
    ANSEL_COLUMN1 = 0;
    ANSEL_COLUMN2 = 0;
    ANSEL_COLUMN3 = 0;

//    ANSEL_ROW1 = 0;
    ANSEL_ROW2 = 0;
    ANSEL_ROW3 = 0;
    ANSEL_ROW4 = 0;

    LAT_COLUMN1 = LOW;
    LAT_COLUMN2 = LOW;
    LAT_COLUMN3 = LOW;
}

void GetKeyPressed()
{
    unsigned char k, key = 0xFF, row, column, keypress = 0xFF;

    LAT_COLUMN1 = LOW;
    LAT_COLUMN2 = LOW;
    LAT_COLUMN3 = LOW;

    LAT_ROW1 = HIGH;
    LAT_ROW2 = HIGH;
    LAT_ROW3 = HIGH;
    LAT_ROW4 = HIGH;

    //READ ROW PIN
    for(column = 1; column <= 3; column++)
    {
        //OUTPUT PIN
        TRIS_COLUMN1 = 1;
        TRIS_COLUMN2 = 1;
        TRIS_COLUMN3 = 1;

        TRIS_ROW1 = 1;
        TRIS_ROW2 = 1;
        TRIS_ROW3 = 1;
        TRIS_ROW4 = 1;
        // TURN ON ROW OUTPUT
        switch (column)
        {
            case 1:
                //OUTPUT PIN
                TRIS_COLUMN1 = 0;
                TRIS_COLUMN2 = 1;
                TRIS_COLUMN3 = 1;
                break;
            case 2:
               //OUTPUT PIN
                TRIS_COLUMN1 = 1;
                TRIS_COLUMN2 = 0;
                TRIS_COLUMN3 = 1;
                break;
            case 3:
                //OUTPUT PIN
                TRIS_COLUMN1 = 1;
                TRIS_COLUMN2 = 1;
                TRIS_COLUMN3 = 0;
                break;
        }

        //READ COLUMNS - BREAK WHEN KEY PRESS DETECTED
        if(ROW1 == 0)
        {
            key = 0*3+(column-1);
//            while(ROW1 == 0);
            break;
        }
        if(ROW2 == 0)
        {
            key = 1*3+(column-1);
//            while(ROW2 == 0);
            break;
        }
        if(ROW3 == 0)
        {
            key = 2*3+(column-1);
//            while(ROW3 == 0);
            break;
        }
        if(ROW4 == 0)
        {
            key = 3*3+(column-1);
//            while(ROW4 == 0);
            break;
        }

    }
    keypress = keyPadMatrix[key];
    if(key!= 0xFF)
        push_i2c_reply_status(KEYPRESS_REG, keypress);

    key = 0xFF;
}


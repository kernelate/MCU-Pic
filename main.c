/*
 * File:   main.c
 * Author: ntekcom27
 *
 * Created on August 24, 2015, 2:41 PM
 * EDITED ON: JUN 14 2016
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "watchdog.h"
#include "i2c_Api.h"
#include "i2c_registers.h"
#include "debug.h"
#include "gpio.h"
#include "peripherals.h"
#include "peripheral_parameters.h"
//#include "timer0.h"
#include "internal_clock_Api.h"
#include "CS6422_Api.h"
#include "keypad_Api.h"

#define _XTAL_FREQ      4000000

int debug_reg = 0x00;
unsigned char debug_msb = 0x00;
unsigned char debug_lsb = 0x00;
//int door_status = 0;
//sensor enable flags



int shutdown_flag = 0;
int shutdown_counter = 0;

void init_gpio();
void init_mcu();

void auto_on();
void shutdown_service();

void interrupt InterruptServiceRoutine() {
    /*if (PIR1bits.SSPIF == 1)
     * {
        work_i2c_handler();
    }*/
    //*
    if (PIR1bits.SSPIF == 1 && SSPSTATbits.S == 1 && i2c_start_flag == 0) {
        work_i2c_handler();

    }
    PIR1bits.SSPIF = 0;
    if (i2c_start_flag = 0 && SSPSTATbits.S == 0) {
        SSPCON1bits.CKP = 1;
    }

    if (PIE1bits.ADIE == 1 && PIR1bits.ADIF) //clear adc interrupt flag
    {
        adc_flag = adc_flag++; // from reading state to read state;
        PIR1bits.ADIF = 0;
    }
    //*/

    /*
    INTCONbits.TMR0IF = 0;
    TMR0 = OVERFLOW_COUNTER;*/
}



int main(int argc, char** argv) {
    int mtn_prescale = 0;
    int shk_prescale = 0;
    int sen0_prescale = 0;
    int sen1_prescale = 0;
    int i2c_prescaler_ctr = 0;
    int adc_prescale = 0;
    /*
    int temp = 0;
    int i2c_data;
    int led_prescale = 0;
    int led_ctr = 0;
    int led_off = 0;
    int led_flag = 0;
     */
    init_mcu();
#ifdef AUTO_ON
    __delay_ms(14000); //initial waiting time
    auto_on();
#endif
    while (1) {
#ifdef DEBUG_MODE
        GPIO_LED_LOCK = 1;
        GPIO_LED_UNLOCK = 1;
        GPIO_RLY0 = 1;
        GPIO_RLY1 = 1;
        GPIO_LED_LIGHT = 1;
        __delay_ms(2000);
        GPIO_LED_LOCK = 0;
        GPIO_LED_UNLOCK = 0;
        GPIO_RLY0 = 0;
        GPIO_RLY1 = 0;
        GPIO_LED_LIGHT = 0;
        __delay_ms(2000);
#endif
        //GPIO_LED_LOCK=LED_ON;
        //GPIO_LED_UNLOCK=LED_OFF;

#ifndef DEBUG_MODE
#if KEYPAD
        if(keypad_en == 1)
        {
            GetKeyPressed();
        }
#endif
#ifdef SENSOR
        if (sensor0_en >= 1) {
            if (sen0_prescale > 100) {
                sensor0_detect();
                sen0_prescale = 0;
            } else {
                sen0_prescale++;
            }
        }
        if (sensor1_en >= 1) {
            if (sen1_prescale > 100) {
                sensor1_detect();
                sen1_prescale = 0;
            } else {
                sen1_prescale++;
            }
        }
#endif
#ifdef MOTION
        if (motion_en >= 1) {
            if (mtn_prescale > 100) {
                motion_detect();
                mtn_prescale = 0;
            } else {
                mtn_prescale++;
            }
        }
#endif
#if SHOCK
        /*
                if (shk_prescale > 100)
                {
                    shock_detect();
                    shk_prescale = 0;
                } else
                {
                    shk_prescale++;
                }
         * */
#endif
#if LDR_IR
        if (ldr_en == 1) {
            if (adc_prescale >8000) {
                adc_read_service();
                adc_prescale = 0;
            } else {
                adc_prescale++;
            }
        }
        if (ir_en == 1) {
            ir_level_service();
        }
#endif
        i2c_interrupt_handler();
#ifdef WATCHDOG
        cpu_watchdog_handler();
#endif
        shutdown_service();
#ifdef I2C_COMMANDS
        if (i2c_prescaler_ctr > 100) {
            i2c_databyte_handler();
            i2c_dataword_handler();
            i2c_prescaler_ctr = 0;
        }
        i2c_prescaler_ctr++;
#endif

#endif
    }
    return (EXIT_SUCCESS);
}

void i2c_databyte_handler() {
    int data;
    int reg = 0xFF;
    reg = get_i2c_regadd_from_master_byte();
    data = get_i2c_msg_from_master_byte();
    switch (i2c_reg_add_byte[reg]) {
        case RELAY0_REG:
            if(relay0_en == 0){
                if (data == 0x01) {
                    GPIO_RLY0 = RLY_ACTIVE;
                    relay0_status = 1;
                } else {
                    if (data == 0x00) {
                        GPIO_RLY0 = RLY_INACTIVE;
                        relay0_status = 0;
                    }
                }
            } else {
                if (data == 0x01) {
                    GPIO_RLY0 = RLY_INACTIVE;
                    relay0_status = 1;
                } else {
                    if (data == 0x00) {
                        GPIO_RLY0 = RLY_ACTIVE;
                        relay0_status = 0;
                    }
                }
            }
            break;
        case RELAY1_REG:
            if(relay1_en == 0){
                if (data == 0x01) {
                    GPIO_RLY1 = RLY_ACTIVE;
                    relay1_status = 1;
                } else {
                    if (data == 0x00) {
                        GPIO_RLY1 = RLY_INACTIVE;
                        relay1_status = 0;
                    }
                }
            } else {
                if (data == 0x01) {
                    GPIO_RLY1 = RLY_INACTIVE;
                    relay1_status = 1;
                } else {
                    if (data == 0x00) {
                        GPIO_RLY1 = RLY_ACTIVE;
                        relay1_status = 0;
                    }
                }
            }
            break;
        case LED_LIGHT_REG:
            if (data == 0x01) {
                GPIO_LED_LIGHT = LED_ON;
            } else {
                if (data == 0x00) {
                    GPIO_LED_LIGHT = LED_OFF;
                }
            }
            break;
        case ECHO_POWER_REG:
            if (data == 0x01) {
                echo_cancel_on();
            } else {
                if (data == 0x00) {
                    echo_cancel_off();
                } else {
                    if (data == 0x02) {
                        echo_cancel_off();
                        echo_cancel_on();
                    }
                }
            }
            break;
        case LED_RED_REG:
            if (data == 0x01) {
                GPIO_LED_LOCK = LED_ON;
            } else {
                if (data == 0x00) {
                    GPIO_LED_LOCK = LED_OFF;
                }
            }
            break;
        case LED_GREEN_REG:
            if (data == 0x01) {
                GPIO_LED_UNLOCK = LED_ON;
            } else {
                if (data == 0x00) {
                    GPIO_LED_UNLOCK = LED_OFF;
                }
            }
            break;
        case LED_FLASH_EN1_REG:
            if (data == 0x01) {
                GPIO_LED_FLASH_EN1 = LED_ON;
            } else {
                if (data == 0x00) {
                    GPIO_LED_FLASH_EN1 = LED_OFF;
                }
            }
            break;
        case LED_FLASH_EN2_REG:
            if (data == 0x01) {
                GPIO_LED_FLASH_EN2 = LED_ON;
            } else {
                if (data == 0x00) {
                    GPIO_LED_FLASH_EN2 = LED_OFF;
                }
            }
            break;
        case LED_FLASH_BOTH_REG:
            if (data == 0x01) {
                GPIO_LED_FLASH_EN1 = LED_ON;
                GPIO_LED_FLASH_EN2 = LED_ON;
            } else {
                if (data == 0x00) {
                    GPIO_LED_FLASH_EN1 = LED_OFF;
                    GPIO_LED_FLASH_EN2 = LED_OFF;
                }
            }
            break;
        case LED_PHONE_BUTTON_REG:
            if (data == 0x01) {
                GPIO_PHONE_BUTTON = LED_ON;
            } else {
                if (data == 0x00) {
                    GPIO_PHONE_BUTTON = LED_OFF;
                }
            }
            break;
        case SENSOR0_EN_REG:
            if (data == 0x01) {
                if (sensor0_en == 0) {
                    init_sensor0();
                }
                sensor0_en = 1;
            } else if (data == 0x02) {
                if (sensor0_en == 0) {
                    init_sensor0();
                }
                sensor0_en = 2;
            } else {
                if (data == 0x00) {
                    if (sensor0_en == 1) {
                        deinit_sensor0();
                    }
                    sensor0_en = 0;
                }
            }
            break;
        case SENSOR1_EN_REG:
            if (data == 0x01) {
                if (sensor1_en == 0) {
                    init_sensor1();
                }
                sensor1_en = 1;
            } else if (data == 0x02) {
                if (sensor1_en == 0) {
                    init_sensor1();
                }
                sensor1_en = 2;
            } else {
                if (data == 0x00) {
                    if (sensor1_en == 1) {
                        deinit_sensor1();
                    }
                    sensor1_en = 0;
                }
            }
            break;
        case MOTION_EN_REG:
            if (data == 0x01) {
                if (motion_en == 0) {
                    init_motion();
                }
                motion_en = 1;
            } else if (data == 0x02) {
                if (motion_en == 0) {
                    init_motion();
                }
                motion_en = 2;
            } else {
                if (data == 0x00) {
                    if (motion_en >= 1) {
                        deinit_motion();
                    }
                    motion_en = 0;
                }
            }
            break;
        case RELAY0_EN_REG:
            if (data == 0x01) {
                relay0_en = 1;
            } else {
                relay0_en = 0;
            }
            break;
        case RELAY1_EN_REG:
            if (data == 0x01) {
                relay1_en = 1;
            } else {
                relay1_en = 0;
            }
            break;
        case AUDIOSW_REG:
            if (data == 0x01) {
                //flag_mosfet = 1;
                GPIO_MIC_AMP = 0; //RD4
                GPIO_AUDIO_AMP = 0; //RD6
                GPIO_HANDSET = 0; //RD5
                //GPIO_AUDIO_SW = AUIDOSW_HIGH;   //HANDSET
            } else {

                if (data == 0x00) {
                    //flag_mosfet = 0;
                    GPIO_MIC_AMP = 1; //GPIOB
                    GPIO_AUDIO_AMP = 1; //GPIOA
                    GPIO_HANDSET = 1; //GPIOC
                    //GPIO_AUDIO_SW = AUIDOSW_LOW;    //SPEAKER
                }
            }
            break;
        case SHUTDOWN_REG:
            if (data == 0x01) {
                GPIO_ON = 1;
                shutdown_flag = 1;
            } else {
                GPIO_ON = 0;
                shutdown_flag = 0;
            }
            break;
        case LED_ONLINE_REG:
            if (data == 0x01) {
                GPIO_LED_OL = LED_ON;
            } else {
                if (data == 0x00) {
                    GPIO_LED_OL = LED_OFF;
                }
            }
            break;
        case SENSOR0_SENSE_REG:
            if (data <= 100) {
                sensor0_sensitivity = data;
            } else {
                sensor0_sensitivity = 100;
            }
            break;
        case SENSOR1_SENSE_REG:
            if (data <= 100) {
                sensor0_sensitivity = data;
            } else {
                sensor0_sensitivity = 100;
            }
            break;
        case MOTION_SENSE_REG:
            if (data <= 100) {
                motion_sensitivity = data;
            } else {
                motion_sensitivity = 100;
            }
            break;
        case LDR_EN_REG:
            if (data == 0x01) {
                if (ldr_en == 0) {
                    ldr_en = 1;
                    init_ldr();
                }
            } else {
                if (ldr_en == 1) {
                    ldr_en = 0;
                    deinit_ldr();
                }
            }
            break;
        case IR_LEVEL_REG:
            if (data <= 3) {
                ir_level = data;
            } else {
                ir_level = 3;
            }
            break;
        case LDR_SENSE1_REG:
            if (data <= 100) {
                ldr_sensitivity1 = data;
            } else {
                ldr_sensitivity1 = 100;
            }
            break;
        case LDR_SENSE2_REG:
            if (data <= 100) {
                ldr_sensitivity2 = data;
            } else {
                ldr_sensitivity2 = 100;
            }
            break;
        case LDR_SENSE3_REG:
            if (data <= 100) {
                ldr_sensitivity3 = data;
            } else {
                ldr_sensitivity3 = 100;
            }
            break;
        case IR_EN_REG:
            if (data == 1) {
                ir_en = 1;
                init_ir();

            } else {
                if (data == 0) {
                    ir_en = 0;
                    deinit_ir();
                }
            }
            break;
        case KEYPAD_EN_REG:
            if(data == 0x01)
            {
                keypad_en = 1;
            }
            else
            {
                if(data == 0){
                    deinit_keypad();
                    keypad_en = 0;
                }
            }
            break;

        default:
            break;
    }
}

//*

void i2c_dataword_handler() {
    int data_msb, data_lsb;
    int reg = 0xFF;
    reg = get_i2c_regadd_from_master_word();
    data_msb = get_i2c_msg_from_master_word_msb();
    data_lsb = get_i2c_msg_from_master_word_lsb();
    debug_reg = reg;
    debug_msb = data_msb;
    debug_lsb = data_lsb;
    switch (i2c_reg_add_word[reg]) {
        case ECHO_REG0:
            ec_msb[0] = data_msb;
            ec_lsb[0] = data_msb;
            cs6422_write_word(data_msb, data_lsb);
            break;
        case ECHO_REG1:
            ec_msb[1] = data_msb;
            ec_lsb[1] = data_msb;
            cs6422_write_word(data_msb, data_lsb);
            break;
        case ECHO_REG2:
            ec_msb[2] = data_msb;
            ec_lsb[2] = data_msb;
            cs6422_write_word(data_msb, data_lsb);
            break;
        case ECHO_REG3:
            ec_msb[3] = data_msb;
            ec_lsb[3] = data_msb;
            cs6422_write_word(data_msb, data_lsb);
            break;
        case ECHO_REG4:
            ec_msb[4] = data_msb;
            ec_lsb[4] = data_msb;
            cs6422_write_word(data_msb, data_lsb);
            break;
        case ECHO_REG5:
            ec_msb[5] = data_msb;
            ec_lsb[5] = data_msb;
            cs6422_write_word(data_msb, data_lsb);
            break;
        default:
            break;
    }
}
//*/



void init_gpio() {
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00;
    init_auto_on();
    init_relay1();
    init_relay2();
    init_led_light();
    init_led_flash_en1();
    init_led_flash_en2();
    init_led_ol();
    init_led_lock();
    init_led_unlock();
    init_led_phone_button();
    init_audio_switch();
#if KEYPAD
    init_keypad();
#endif
}

void init_mcu() {
    init_internal_clock();
    init_gpio();
//    init_cs6422();
    init_i2c();
    //init_timer0();
    //echo_cancel_off();
    //echo_cancel_on();
}


void auto_on() {

    GPIO_ON = ON_ACTIVE;
    //PORTDbits.RD2 = 1;
    __delay_ms(5000);
    GPIO_ON = ON_INACTIVE;
    //__delay_ms(5000);
    //PORTDbits.RD2 = 0;
}

void shutdown_service() {
    if (shutdown_flag == 1) {
        if (shutdown_counter > 5000) {
            GPIO_ON = 0;
            shutdown_counter = 0;
            shutdown_flag = 0;
        } else {
            GPIO_ON = 1;
            shutdown_counter++;
        }
    }
}



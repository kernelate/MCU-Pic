#include <xc.h>
#include "gpio.h"
#include "peripherals.h"
#include "peripheral_parameters.h"
#include "debug.h"
#include "i2c_registers.h"
#include "i2c_Api.h"

#define _XTAL_FREQ 4000000
void init_sensor0()
{
    TRIS_SENSOR0 = 1;
    GPIO_SENSOR0 = 0;
    ANSEL_SENSOR0 = 0;
}

void init_sensor1()
{
    TRIS_SENSOR1 = 1;
    GPIO_SENSOR1 = 0;
    ANSEL_SENSOR1 = 0;
}

void init_motion()
{
    TRIS_MOTION = 1;
    GPIO_MOTION = 0;
    ANSEL_MOTION = 0;
}

void deinit_sensor0()
{
    TRIS_SENSOR0 = 0;
    GPIO_SENSOR0 = 0;
    ANSEL_SENSOR0 = 0;
}

void deinit_sensor1()
{
    TRIS_SENSOR1 = 0;
    GPIO_SENSOR1 = 0;
    ANSEL_SENSOR1 = 0;
}

void deinit_motion()
{
    TRIS_MOTION = 0;
    GPIO_MOTION = 0;
    ANSEL_MOTION = 0;
}

void init_relay1()
{
    TRIS_RLY0 = 0;
    GPIO_RLY0 = RLY_INACTIVE;
    ANSEL_RLY0 = 0;
}

void init_relay2()
{
    TRIS_RLY1 = 0;
    GPIO_RLY1 = RLY_INACTIVE;
    ANSEL_RLY1 = 0;
}

void init_led_light()
{
    TRIS_LED_LIGHT = 0;
    GPIO_LED_LIGHT = LED_OFF;
    ANSEL_LED_LIGHT = 0;
}

void init_led_flash_en1()
{
    TRIS_LED_FLASH_EN1 = 0;
    GPIO_LED_FLASH_EN1 = 0;
    // ANSEL_LED_FLASH_EN1 = 0;
}

void init_led_flash_en2()
{
    TRIS_LED_FLASH_EN2 = 0;
    GPIO_LED_FLASH_EN2 = 0;
    ANSEL_LED_FLASH_EN2 = 0;
}

void init_led_ol()
{
    TRIS_LED_OL = 0;
    GPIO_LED_OL = 0;
    //    ANSEL_LED_OL = 0;
}

void init_led_lock()
{
    TRIS_LED_LOCK = 0;
    GPIO_LED_LOCK = LED_OFF;
    //ANSEL_LED_LOCK = 0;
}

void init_led_unlock()
{
    TRIS_LED_UNLOCK = 0;
    GPIO_LED_UNLOCK = LED_OFF;
    //ANSEL_LED_UNLOCK = 0;
}

void init_auto_on()
{
    TRIS_ON = 0;
    GPIO_ON = ON_INACTIVE;
    ANSEL_ON = 0;
}

void init_led_phone_button()
{
    TRIS_PHONE_BUTTON = 0;
    GPIO_PHONE_BUTTON = 0;
    ANSEL_PHONE_BUTTON = 0;
}

void init_audio_switch()
{
    TRIS_MIC_AMP = 0;
    GPIO_MIC_AMP = 1; //GPIOB
    ANSEL_MIC_AMP = 0;
    TRIS_AUDIO_AMP = 0;
    GPIO_AUDIO_AMP = 1; //GPIOA
    ANSEL_AUDIO_AMP = 0;
    TRIS_HANDSET = 0;
    GPIO_HANDSET = 1; //GPIOC
    ANSEL_HANDSET = 0;
}

void init_ldr()
{
    ADCON0 = 0x01; // enable ADC module
    ADCON1 = 0x70; // Right Justified output format, use internal FRC oscillator
    // use Vdd as voltage reference
    //enable adc_interrupt
    PIR1bits.ADIF = 0; //ADC Interrup Flag
    PIE1bits.ADIE = 0; // ADC Enable Interrupt
    INTCONbits.GIE = 1; //Global Interrupt
    INTCONbits.PEIE = 1; // Peripheral interrupt

    LDR_TRIS = 1;
    LDR_GPIO = 0;
    LDR_ANSEL = 1;
}

void deinit_ldr()
{
    ADCON0 = 0x00; // enable ADC module
    PIR1bits.ADIF = 0; //ADC Interrup Flag
    PIE1bits.ADIE = 0; // ADC Enable Interrupt
    LDR_TRIS = 0;
    LDR_GPIO = 0;
    LDR_ANSEL = 0;
}

void init_ir()
{
    IR1_TRIS = 0;
    IR1_GPIO = 0;
    // IR1_ANSEL = 0;

    IR2_TRIS = 0;
    IR2_GPIO = 0;
    IR2_ANSEL = 0;

    IR3_TRIS = 0;
    IR3_GPIO = 0;
    // IR3_ANSEL = 0;
}

void deinit_ir()
{
    IR1_TRIS = 0;
    IR1_GPIO = 0;
    //IR1_ANSEL = 0;

    IR2_TRIS = 0;
    IR2_GPIO = 0;
    IR2_ANSEL = 0;

    IR3_TRIS = 0;
    IR3_GPIO = 0;
    // IR3_ANSEL = 0;
}

void ir_level_service()
{
    switch (ir_level)
    {
        case 1:
            IR1_GPIO=0;
            IR2_GPIO=1;
            IR3_GPIO=0;
            break;
        case 2:
            IR1_GPIO=1;
            IR2_GPIO=0;
            IR3_GPIO=1;
            break;
        case 3:
            IR1_GPIO=1;
            IR2_GPIO=1;
            IR3_GPIO=1;
            break;
        default:
            IR1_GPIO=0;
            IR2_GPIO=0;
            IR3_GPIO=0;
            break;

    }
}


#ifdef SHOCK
int shock_counter = 0;

int shock_detect() {/*
    if (GPIO_SHOCK == SHOCK_ACTIVE)
    {
        if (shock_counter < SHOCK_SENSITIVITY)
        {
            shock_counter++;
        } else
        {
            GPIO_LED_UNLOCK = ~GPIO_LED_UNLOCK;
            push_i2c_reply_status(SHOCK_REG, I2C_SHOCK_MSG);
        }
    } else
    {
        shock_counter = 0;
    }
  * */
}
#endif
#ifdef MOTION
int motion_counter = 0;

int motion_detect() {
    if (motion_en == 1) {
        if (GPIO_MOTION == MOTION_ACTIVE) {
            if (motion_counter < motion_sensitivity) {
                motion_counter++;
            } else {
                push_i2c_reply_status(MOTION_REG, I2C_MOTION_ON);
                motion_counter = 0;
            }
        } else {
            motion_counter = 0;
        }
    } else if (motion_en == 2) {
        if (GPIO_MOTION == MOTION_INACTIVE) {
            if (motion_counter < motion_sensitivity) {
                motion_counter++;
            } else {
                push_i2c_reply_status(MOTION_REG, I2C_MOTION_ON);
                motion_counter = 0;
            }
        } else {
            motion_counter = 0;
        }
    }


}
#endif
#ifdef SENSOR
int sensor1_counter = 0;

int sensor1_detect() {
    if (sensor1_en == 1){
        if (GPIO_SENSOR1 == SENSOR_ACTIVE) {
            if (sensor1_counter < sensor1_sensitivity) {
                sensor1_counter++;
            } else {
                if (sensor1_status == 0) {
                    push_i2c_reply_status(SENSOR1_REG, I2C_SENSE1_ON);
                    sensor1_status = 1;
                }
                sensor1_counter = 0;
            }
        } else {
            sensor1_counter = 0;
            if (sensor1_status == 1) {
                push_i2c_reply_status(SENSOR1_REG, I2C_SENSE1_OFF);
                sensor1_status = 0;
            }
        }
    } else if (sensor1_en == 2){
        if (GPIO_SENSOR1 == SENSOR_INACTIVE) {
            if (sensor1_counter < sensor1_sensitivity) {
                sensor1_counter++;
            } else {
                if (sensor1_status == 0) {
                    push_i2c_reply_status(SENSOR1_REG, I2C_SENSE1_ON);
                    sensor1_status = 1;
                }
                sensor1_counter = 0;
            }
        } else {
            sensor1_counter = 0;
            if (sensor1_status == 1) {
                push_i2c_reply_status(SENSOR1_REG, I2C_SENSE1_OFF);
                sensor1_status = 0;
            }
        }
    }
    
}
#endif

#ifdef SENSOR
int sensor0_counter = 0;

int sensor0_detect() {
    if(sensor0_en == 1){
        if (GPIO_SENSOR0 == SENSOR_ACTIVE) {
            if (sensor0_counter < sensor0_sensitivity) {
                sensor0_counter++;
            } else {
                if (sensor0_status == 0) {
                    push_i2c_reply_status(SENSOR0_REG, I2C_SENSE0_ON);
                    sensor0_status = 1;
                }
                sensor0_counter = 0;
            }
        } else {
            sensor0_counter = 0;
            if (sensor0_status == 1) {
                push_i2c_reply_status(SENSOR0_REG, I2C_SENSE0_OFF);
                sensor0_status = 0;
            }
        }
    } else if (sensor0_en == 2){
        if (GPIO_SENSOR0 == SENSOR_INACTIVE) {
            if (sensor0_counter < sensor0_sensitivity) {
                sensor0_counter++;
            } else {
                if (sensor0_status == 0) {
                    push_i2c_reply_status(SENSOR0_REG, I2C_SENSE0_ON);
                    sensor0_status = 1;
                }
                sensor0_counter = 0;
            }
        } else {
            sensor0_counter = 0;
            if (sensor0_status == 1) {
                push_i2c_reply_status(SENSOR0_REG, I2C_SENSE0_OFF);
                sensor0_status = 0;
            }
        }
    }
    
}

#endif
#ifdef DOOR_DET

int door_detect() {
    /*
    if (GPIO_DOOR == 0) {

        if (GPIO_SENSOR0 == SENSOR_ACTIVE) {
            GPIO_LED_LOCK = LED_ON;
            GPIO_LED_UNLOCK = LED_OFF;
        } else {
            GPIO_LED_LOCK = LED_OFF;
            GPIO_LED_UNLOCK = LED_ON;
        }
    } else {
        GPIO_LED_LOCK = LED_OFF;
        GPIO_LED_UNLOCK = LED_OFF;
    }
     */
    /*
    if (GPIO_DOOR == door_status)
    {

    } else
    {
        if (GPIO_DOOR == 0)
        {
            push_i2c_reply_status(DOOR_SENSOR_REG, I2C_DOOR_SENSOR_PRESENT);
        } else
        {
            push_i2c_reply_status(DOOR_SENSOR_REG, I2C_DOOR_SENSOR_ABSENT);

        }
    }
     */
}
#endif

void read_ldr() {

    ADCON0 &= 0x83; //clears analog selection bits
    ADCON0 |= LDR_CNL << 2; //set channel
    __delay_ms(2);
    GO_nDONE = 1; //start ADC conversion
}

void adc_read_service() {
    switch (adc_flag) {
        case LDR_READ:
            read_ldr();
            adc_flag++;
            break;
        case LDR_READING:
            //do nothing interrupt will add to adc_flag when done
            if (GO_nDONE == 0) {
                adc_flag++;
            }
            break;
        case LDR_READ_DONE:
            ldr_value = (int)(ADRESH / 1.6);
            push_i2c_reply_status(LDR_READ_REG, ldr_value);
            adc_flag++;
            break;

        default:
            adc_flag = 1;
            break;
    }

}

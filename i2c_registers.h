/*
 * Created by: Charles O
 * Edited by: Charles O
 * Date Edited: June 14, 2016
 *
 * RegisterAddress For i2c and data are put here
 * RegisterAddress Arrays are different for
 * data size byte and data size word BUT NO address should be the same.
 */
//REGISTER ADDRESS FOR BYTE DATA
#define READ_REG_BYTE           0xAA
#define AUDIOSW_REG             0xBB
#define RELAY0_REG              0xBC
#define RELAY1_REG              0xBD
#define LED_LIGHT_REG           0XBE
#define ECHO_POWER_REG          0xC0
#define SENSOR0_REG             0xE1
#define SENSOR1_REG             0xE2
#define MOTION_REG              0xE3
#define SHOCK_REG               0xE4
#define LED_RED_REG             0xE5
#define LED_GREEN_REG           0xE6
#define DOOR_SENSOR_REG         0xE7
#define LED_FLASH_EN1_REG       0xE8
#define LED_FLASH_EN2_REG       0xE9
#define LED_FLASH_BOTH_REG      0xEA
#define LED_PHONE_BUTTON_REG    0xEB
#define SENSOR0_EN_REG          0x30
#define SENSOR1_EN_REG          0x31
#define MOTION_EN_REG           0x32
#define LED_ONLINE_REG          0x33
#define LDR_EN_REG              0x34
#define IR_EN_REG               0x36
#define RELAY0_EN_REG           0x37
#define RELAY1_EN_REG           0x38
#define SENSOR0_SENSE_REG       0X40
#define SENSOR1_SENSE_REG       0X41
#define MOTION_SENSE_REG        0X42
#define IR_LEVEL_REG            0x50
#define LDR_SENSE1_REG          0x51
#define LDR_SENSE2_REG          0x52
#define LDR_SENSE3_REG          0x53
#define LDR_READ_REG            0x60
#define SHUTDOWN_REG            0x90
#define KEYPAD_EN_REG           0x70
#define KEYPRESS_REG            0x71



#define MAX_REG_ADD_BYTE     36
#define MAX_REG_BUFF_BYTE    1
unsigned char i2c_reg_add_byte[MAX_REG_ADD_BYTE] = {
    READ_REG_BYTE, //DO NOT REMOVE THIS
    AUDIOSW_REG,
    RELAY0_REG,
    RELAY1_REG,
    LED_LIGHT_REG,
    ECHO_POWER_REG,
    SENSOR0_REG,
    SENSOR1_REG,
    MOTION_REG,
    SHOCK_REG,
    LED_RED_REG,
    LED_GREEN_REG,
    DOOR_SENSOR_REG,
    LED_FLASH_EN1_REG,
    LED_FLASH_EN2_REG,
    LED_FLASH_BOTH_REG,
    LED_PHONE_BUTTON_REG,
    SENSOR0_EN_REG,
    SENSOR1_EN_REG,
    MOTION_EN_REG,
    LDR_EN_REG,
    IR_EN_REG,
    RELAY0_EN_REG,
    RELAY1_EN_REG,
    LED_ONLINE_REG,
    SENSOR0_SENSE_REG,
    SENSOR1_SENSE_REG,
    MOTION_SENSE_REG,
    IR_LEVEL_REG,
    LDR_SENSE1_REG,
    LDR_SENSE2_REG,
    LDR_SENSE3_REG,
    LDR_READ_REG,
    SHUTDOWN_REG,
    KEYPAD_EN_REG,
    KEYPRESS_REG,
};
unsigned char i2c_reg_databyte[MAX_REG_ADD_BYTE][MAX_REG_BUFF_BYTE];









//REGISTER ADDRESS FOR WORD DATA
#define ECHO_REG0   0xD0
#define ECHO_REG1   0xD1
#define ECHO_REG2   0xD2
#define ECHO_REG3   0xD3
#define ECHO_REG4   0xD4
#define ECHO_REG5   0xD5

#define MAX_REG_ADD_WORD     6
#define MAX_REG_BUFF_WORD    1
unsigned char i2c_reg_add_word[MAX_REG_ADD_WORD] = {
    ECHO_REG0,
    ECHO_REG1,
    ECHO_REG2,
    ECHO_REG3,
    ECHO_REG4,
    ECHO_REG5
};

unsigned char i2c_reg_dataword_msb[MAX_REG_ADD_WORD][MAX_REG_BUFF_WORD];
unsigned char i2c_reg_dataword_lsb[MAX_REG_ADD_WORD][MAX_REG_BUFF_WORD];


//NOTE: DATA FOR REGISTERS
//Tx
#define I2C_MSG_KEEPALIVE       0xFF

#define I2C_SHOCK_MSG           0x01
#define I2C_DOOR_OPEN           0x01
#define I2C_DOOR_CLOSE          0x00
#define I2C_SENSE1_ON           0x01
#define I2C_SENSE1_OFF          0x00
#define I2C_SENSE0_ON           0x01
#define I2C_SENSE0_OFF          0x00
#define I2C_MOTION_ON           0x01
#define I2C_DOOR_SENSOR_PRESENT 0x01
#define I2C_DOOR_SENSOR_ABSENT  0x00
//Rx
#define I2C_MSG_NO_DATA         0xFB
#define I2C_RLY0_ON             0x01
#define I2C_RLY0_OFF            0x03
#define I2C_RLY1_ON             0x05
#define I2C_RLY1_OFF            0x09
#define I2C_LED_ON              0x0B
#define I2C_LED_OFF             0x0D

#define I2C_ECHO_REG0           0x00
#define I2C_ECHO_REG1           0x02
#define I2C_ECHO_REG2           0x04
#define I2C_ECHO_REG3           0x08
#define I2C_ECHO_REG4           0x0A
#define I2C_ECHO_ON             0x0C
#define I2C_ECHO_OFF            0x0E
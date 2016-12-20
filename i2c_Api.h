/*
 * File:   i2c_Api.h
 * Created on June 1, 2016
 */

#define SLAVE_ADDRESS       0x33
#define MAX_I2C_BUFFER      7
//#define MAX_MEM_MAP         0x20
//#define MAX_INT_RESET       1000

#define DATA_I2C_BIT        1
#define ADDR_I2C_BIT        0
#define READ_I2C_BIT_INFO   1
#define WRITE_I2C_BIT_INFO  0

#define I2C_SDA                 PORTCbits.RC4
#define I2C_SCL                 PORTCbits.RC3
#define I2C_INT                 PORTCbits.RC2

void init_i2c(void);

//#define I2C_MASTER
#define I2C_SLAVE

#ifdef I2C_MASTER
int i2c_read();
int i2c_write(char data);
#endif

#ifdef I2C_SLAVE
void work_i2c_handler();
int push_i2c_reply_status(unsigned char reg_add, unsigned char i2c_event);
unsigned char i2c_read_handler(int reg);
unsigned char pop_i2c_reply_status(int reg);
void i2c_interrupt_handler();
unsigned char i2c_read_handler_word(int reg);

//byte functions
int get_i2c_regadd_from_master_byte();
unsigned char get_i2c_msg_from_master_byte();

//master writes function
int verify_byte_address(unsigned char register_add);
void read_databyte(int reg);
void i2c_write_handler_byte(int reg, unsigned char i2c_data);
void i2c_databyte_handler();

//master reads function
void write_databyte(int reg);

//word functions
int get_i2c_regadd_from_master_word();
unsigned char get_i2c_msg_from_master_word_lsb();
unsigned char get_i2c_msg_from_master_word_msb();
//master writes
int verify_word_address(unsigned char register_add);
void read_dataword(int reg);
void i2c_write_handler_word(int reg, unsigned char i2c_data_msb, unsigned char i2c_data_lsb);
void i2c_dataword_handler();
//master reads
void write_dataword(int reg);
#endif
#define MAX_I2C_TX_BUFFER 6



int i2c_start_flag = 0;
int relay0_status = 0;
int relay1_status = 0;

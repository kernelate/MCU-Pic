
/*NOTE:
 *      THIS IS THE REVISED I2C
 */
#include <xc.h>
#include <string.h>
#include "watchdog.h"
#include "i2c_Api.h"
#include "i2c_registers.h"
#include "debug.h"
#include "peripherals.h"
#ifdef I2C_DEBUG
unsigned char i2c_debug_flow[15] = {
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
    , '0', '0', '0', '0', '0'
};
#endif
#ifdef I2C_DEBUG
int i2c_dbg_ctr = 0;
#endif
int i2c_interrupt_flag = 0;
int i2c_interrupt_prescaler = 0;

unsigned char i2c_tx_radd_buff_byte[MAX_I2C_BUFFER];
unsigned char i2c_tx_data_buff_byte[MAX_I2C_BUFFER];

unsigned char i2c_rx_data_buff_byte[MAX_I2C_BUFFER];
int i2c_rx_radd_buff_byte[MAX_I2C_BUFFER];

unsigned char i2c_tx_data_buff_word[MAX_I2C_BUFFER];

unsigned char i2c_rx_data_buff_word_msb[MAX_I2C_BUFFER];
unsigned char i2c_rx_data_buff_word_lsb[MAX_I2C_BUFFER];
int i2c_rx_radd_buff_word[MAX_I2C_BUFFER];

void init_i2c()
{
    //NOTE: This i2c init was configured to be used with interrupt handler
    SSPADD = SLAVE_ADDRESS << 1; //"<<1" last bit is for R/W bit

    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC2 = 0; //output, for cpu_interrupt
    LATCbits.LATC4 = 1; //both unimplemented
    LATCbits.LATC3 = 1; //both unimplemented
    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC2 = 0; //0-digital, 1-analog

    // Setting of SSPSTAT Register
    SSPSTAT = 0x00; //Initialize value
    SSPSTATbits.SMP = 1; //Slew Rate contol set for 100kHz
    SSPSTATbits.CKE = 1; //enable SMBus

    // Setting of SSPCON1 Register
    SSPCON1 = 0x0E; // SSPM :I2C slave, 7 bit address with start/stop bits
    WCOL = 0; // no collision
    SSPOV = 1;
    SSPEN = 1;
    CKP = 1;

    // Setting of SSPCON2 Register
    SSPCON2 = 0x00; //Initialize value
    ACKDT = 0;
    ACKEN = 1;
    SEN = 1;

    // Setting of SSPCON3 Register
    SSPCON3 = 0x00; //Initialize value
    PCIE = 1;
    SCIE = 1;
    BOEN = 1;
    SDAHT = 0;
    SBCDE = 0; //collision is not used

    // Setting of SSP MASK REGISTER
    SSPMSK = 0x00; //Initialize value
    SSPMSK = 0xfe;

    //    return; //interrupt getting overlap config
    // Setting of INTERRUPT CONTROL REGISTER
    INTCON = 0x00; //Initialize value
    INTCONbits.PEIE = 1; //Enable peripheral interrupt
    INTCONbits.GIE = 1; //TODO: timer0 off when enabled
    // Setting of PERIPHERAL INTERRUPT ENABLE REGISTER 1
    PIE2 = 0x00; //Initialize value
    PIE1bits.SSPIE = 1;

    PIR1bits.SSPIF = 0; //SSPIF
    //SSPBUF=0xFF;
    memset(i2c_tx_radd_buff_byte, I2C_MSG_KEEPALIVE, MAX_I2C_BUFFER);
    memset(i2c_tx_data_buff_byte, I2C_MSG_KEEPALIVE, MAX_I2C_BUFFER);

    memset(i2c_rx_data_buff_byte, I2C_MSG_NO_DATA, MAX_I2C_BUFFER);
    memset(i2c_rx_radd_buff_byte, I2C_MSG_NO_DATA, MAX_I2C_BUFFER * 2);

    memset(i2c_rx_radd_buff_word, I2C_MSG_NO_DATA, MAX_I2C_BUFFER * 2);
    memset(i2c_rx_data_buff_word_lsb, I2C_MSG_NO_DATA, MAX_I2C_BUFFER);
    memset(i2c_rx_data_buff_word_msb, I2C_MSG_NO_DATA, MAX_I2C_BUFFER);

    //memset(i2c_debug_flow, 0, MAX_TEST);
    //i2c_df_ctr = 0;
    I2C_INT = 0;
}

void i2c_int_heartbeat()
{
    //    if()
}

int i2c_data_rcv()
{
    return SSPSTATbits.D_nA;
}

int i2c_bit_info()
{
    return SSPSTATbits.R_nW;
}

int push_i2c_reply_status(unsigned char reg_add, unsigned char i2c_event)
{
    int i;
    int reg;
    reg = verify_byte_address(reg_add);
    if (reg == 0xFF)
    {
        reg = verify_word_address(reg_add);
    }
    if (reg < 0xFF)
    {
        for (i = 0; i < MAX_I2C_TX_BUFFER; i++)
        {
            switch (reg_add)
            {
                case MOTION_REG:
                case LDR_READ_REG:
                case SENSOR0_REG:
                case SENSOR1_REG:
                case KEYPRESS_REG:
                case SHOCK_REG:
                    //overwrite latest data
                    //anti-spam for sensors and input that are only either high or low
                    if (i2c_tx_radd_buff_byte[i] == reg_add)
                    {
                        i2c_tx_radd_buff_byte[i] = reg_add;
                        i2c_tx_data_buff_byte[i] = i2c_event;
                        return 1;
                    }
                default:
                    //do not overwrite latest data/ add new entry
                    //for inputs like remote and keyboard
                    if (i2c_tx_data_buff_byte[i] == I2C_MSG_KEEPALIVE)
                    {
                        i2c_tx_radd_buff_byte[i] = reg_add;
                        i2c_tx_data_buff_byte[i] = i2c_event;
                        i2c_interrupt_flag++;
                        return 1;
                    }
            }
        }
    }
    return 0;
}

void i2c_interrupt_handler()
{
#ifndef I2C_INT_TOGGLE
    if (i2c_interrupt_prescaler > 100)
    {
        if (i2c_interrupt_flag > 0) //i2c_interrupt_flag = no of data needed to be read > 0 means someting needs reading
        {
            //PORTDbits.RD1=1;  //debugging purposes only
            I2C_INT = 1;
        } else
        {
            //PORTDbits.RD1=0;  //debugging purposes only
            I2C_INT = 0;
        }
        i2c_interrupt_prescaler = 0;
    } else
    {
        i2c_interrupt_prescaler++;
    }
#endif
#ifdef I2C_INT_TOGGLE
    if (i2c_interrupt_prescaler > 100)
    {
        if (i2c_interrupt_flag > 0) //i2c_interrupt_flag = no of data needed to be read > 0 means someting needs reading
        {
            //         I2C_INT = 1;
            //         I2C_INT = 0;
        }
        i2c_interrupt_prescaler = 0;
    } else
    {
        i2c_interrupt_prescaler++;
    }
#endif
}
//TODO: revise this block

unsigned char pop_i2c_reply_status(int reg)
{
    int i;
    unsigned char buff;
    //check if address is in "push_reply" buffer
    for (i = 0; i < MAX_I2C_TX_BUFFER; i++)
    {
        if (i2c_tx_radd_buff_byte[i] == i2c_reg_add_byte[reg])
        {
            buff = i2c_tx_data_buff_byte[i];
            i2c_interrupt_flag--;
            break;
        }
    }
    if (i >= MAX_I2C_TX_BUFFER)
    {
        //if not in the "push_reply" check if address is READ_REG_BYTE address
        //ver. 1
        /*
        if (reg == 0) //READ REG BYTE
        {
            if (i2c_tx_radd_buff_byte[0] == 0xFF) {
                buff = 0xFF;
            } else {
                buff = i2c_tx_radd_buff_byte[0]; //THIS WILL SEND THE ADDRESS TO BE READ NEXT
            }
        } else {
            buff = 0xFF;
        }
        //*/
        //ver. 2
        //*
        switch (i2c_reg_add_byte[reg])
        {
            case READ_REG_BYTE:
                if (i2c_tx_radd_buff_byte[0] == 0xFF)
                {
                    buff = 0xFF;
                } else
                {
                    buff = i2c_tx_radd_buff_byte[0]; //THIS WILL SEND THE ADDRESS TO BE READ NEXT
                }
                break;
            case RELAY0_REG:
                buff = relay0_status;
                break;
            case RELAY1_REG:
                buff = relay1_status;
                break;
            case LDR_READ_REG:
                buff = ldr_value;
                break;

            default:
                buff = 0xFF;
                break;
        }
        //*/
    } else
    {
        //ADJUST I2C BUFFER and address
        while (i < (MAX_I2C_TX_BUFFER - 1))
        {
            i2c_tx_radd_buff_byte[i] = i2c_tx_radd_buff_byte[i + 1];
            i2c_tx_data_buff_byte[i] = i2c_tx_data_buff_byte[i + 1];
            i++;
        }
        i2c_tx_radd_buff_byte[i] = I2C_MSG_KEEPALIVE;
        i2c_tx_data_buff_byte[i] = I2C_MSG_KEEPALIVE;
    }
    return buff;
}

unsigned char i2c_read_handler(int reg)
{
    return pop_i2c_reply_status(reg);
}

int get_i2c_regadd_from_master_byte()
{
    int i;
    int buff = i2c_rx_radd_buff_byte[0];
    for (i = 1; i < MAX_I2C_BUFFER; i++)
    {
        i2c_rx_radd_buff_byte[i - 1] = i2c_rx_radd_buff_byte[i];
    }
    i2c_rx_radd_buff_byte[MAX_I2C_BUFFER - 1] = I2C_MSG_NO_DATA;
    return buff;
}

unsigned char get_i2c_msg_from_master_byte()
{
    int i;
    unsigned char buff = i2c_rx_data_buff_byte[0];
    for (i = 1; i < MAX_I2C_BUFFER; i++)
    {
        i2c_rx_data_buff_byte[i - 1] = i2c_rx_data_buff_byte[i];
    }
    i2c_rx_data_buff_byte[MAX_I2C_BUFFER - 1] = I2C_MSG_NO_DATA;
    return buff;
}

void i2c_write_handler_byte(int reg, unsigned char i2c_data)
{
    int i;

    for (i = 0; i < MAX_I2C_BUFFER; i++)
    {
        if (i2c_rx_data_buff_byte[i] == I2C_MSG_NO_DATA)
        {
            i2c_rx_radd_buff_byte[i] = reg;
            i2c_rx_data_buff_byte[i] = i2c_data;
            break;
        }
    }
}

void read_databyte(int reg)
{
    i2c_reg_databyte[reg][0] = SSPBUF;
    i2c_write_handler_byte(reg, i2c_reg_databyte[reg][0]);
}

/*
void i2c_databyte_handler(int reg)
{
    unsigned char data;
    data= i2c_reg_databyte[reg][0];
    switch(i2c_reg_add_byte[reg])
    {
        case RELAY0_REG:
            break;
        case RELAY1_REG:
            break;
        case LIGHT_REG:
            break;
        default:
            break;
    }
}
 */
void write_databyte(int reg)
{
    SSPBUF = i2c_reg_databyte[reg][0];
}

int verify_word_address(unsigned char register_add)
{
    for (int i = 0; i < MAX_REG_ADD_WORD; i++)
    {
        if (register_add == i2c_reg_add_word[i])
        {
            return i;
        }
    }
    return 0XFF;
}

void read_dataword(int reg)
{
    unsigned char lsb = 0x00;
    unsigned char msb = 0x00;
    lsb = SSPBUF;
    SSPCON1bits.CKP = 1;
    PIR1bits.SSPIF = 0;
    while (PIR1bits.SSPIF == 0);
    msb = SSPBUF;
    //i2c_reg_dataword[reg][0] = (msb<<8 | lsb);
    i2c_reg_dataword_msb[reg][0] = msb;
    i2c_reg_dataword_lsb[reg][0] = lsb;
    //debug_msb = msb;
    //debug_lsb = lsb;
    i2c_write_handler_word(reg, i2c_reg_dataword_msb[reg][0], i2c_reg_dataword_lsb[reg][0]);

}

/*/
void i2c_dataword_handler(int reg)
{
    unsigned char data_msb, data_lsb;
    data_msb = i2c_reg_dataword_msb[reg][0];
    data_lsb = i2c_reg_dataword_msb[reg][0];
    switch (i2c_reg_add_word[reg]) {
        case ECHO_REG0:
            break;
        case ECHO_REG1:
            break;
        case ECHO_REG2:
            break;
        case ECHO_REG3:
            break;
        case ECHO_REG4:
            break;
        case ECHO_REG5:
            break;
        default:
            break;
    }
}
//*/


int get_i2c_regadd_from_master_word()
{
    int i;
    int buff = i2c_rx_radd_buff_word[0];
    for (i = 1; i < MAX_I2C_BUFFER; i++)
    {
        i2c_rx_radd_buff_word[i - 1] = i2c_rx_radd_buff_word[i];
    }
    i2c_rx_radd_buff_word[MAX_I2C_BUFFER - 1] = I2C_MSG_NO_DATA;
    return buff;
}

unsigned char get_i2c_msg_from_master_word_lsb()
{
    int i;
    unsigned char buff = i2c_rx_data_buff_word_lsb[0];
    for (i = 1; i < MAX_I2C_BUFFER; i++)
    {

        i2c_rx_data_buff_word_lsb[i - 1] = i2c_rx_data_buff_word_lsb[i];
    }
    i2c_rx_data_buff_word_lsb[MAX_I2C_BUFFER - 1] = I2C_MSG_NO_DATA;
    return buff;
}

unsigned char get_i2c_msg_from_master_word_msb()
{
    int i;
    unsigned char buff = i2c_rx_data_buff_word_msb[0];
    for (i = 1; i < MAX_I2C_BUFFER; i++)
    {
        i2c_rx_data_buff_word_msb[i - 1] = i2c_rx_data_buff_word_msb[i];
    }
    i2c_rx_data_buff_word_msb[MAX_I2C_BUFFER - 1] = I2C_MSG_NO_DATA;
    return buff;
}

void i2c_write_handler_word(int reg, unsigned char i2c_data_msb, unsigned char i2c_data_lsb)
{
    int i;

    for (i = 0; i < MAX_I2C_BUFFER; i++)
    {
        if (i2c_rx_data_buff_word_lsb[i] == I2C_MSG_NO_DATA && i2c_rx_data_buff_word_msb[i] == I2C_MSG_NO_DATA)
        {
            i2c_rx_radd_buff_word[i] = reg;
            i2c_rx_data_buff_word_lsb[i] = i2c_data_lsb;
            i2c_rx_data_buff_word_msb[i] = i2c_data_msb;
            break;
        }
    }
}

int verify_byte_address(unsigned char register_add)
{
    for (int i = 0; i < MAX_REG_ADD_BYTE; i++)
    {
        if (register_add == i2c_reg_add_byte[i])
        {
            return i;
        }
    }
    return 0XFF;
}

void work_i2c_handler() //slave i2c handler
{
    int reg = 0xFF;
    unsigned char i2c_add = 0;
    unsigned char reg_add = 0;
    unsigned char reg_data = 0xFF;
    unsigned char garbage = 0;
#ifdef I2C_DEBUG
    i2c_dbg_ctr = 0;
#endif
    if (PIR1bits.SSPIF == 0)
    {
        return;
    }
    i2c_start_flag = 1;
    if (SSPCON1bits.SSPOV != 0) //error checking bit
    {
        SSPCON1bits.SSPOV = 0;
    }
    //*/

    i2c_add = SSPBUF;
    if (i2c_data_rcv() == ADDR_I2C_BIT)
    {
#ifdef I2C_DEBUG
        i2c_debug_flow[i2c_dbg_ctr] = 'a';
        i2c_dbg_ctr++;
#endif
        if (i2c_add == SLAVE_ADDRESS << 1)
        {
#ifdef I2C_DEBUG
            i2c_debug_flow[i2c_dbg_ctr] = 'b';
            i2c_dbg_ctr++;
#endif
            SSPCON1bits.CKP = 1;
            PIR1bits.SSPIF = 0;
            if (i2c_bit_info() == WRITE_I2C_BIT_INFO)
            {
#ifdef I2C_DEBUG
                i2c_debug_flow[i2c_dbg_ctr] = 'c';
                i2c_dbg_ctr++;
#endif
                while (PIR1bits.SSPIF == 0);
                reg_add = SSPBUF;
                SSPCON1bits.CKP = 1;
                PIR1bits.SSPIF = 0;
                reg = verify_byte_address(reg_add);
#ifndef SOME_ERROR
                //*/
                if (i2c_bit_info() == READ_I2C_BIT_INFO)
                {
#ifdef I2C_DEBUG
                    i2c_debug_flow[i2c_dbg_ctr] = 'R';
                    i2c_dbg_ctr++;
#endif
                }
                if (i2c_bit_info() == WRITE_I2C_BIT_INFO)
                {
#ifdef I2C_DEBUG
                    i2c_debug_flow[i2c_dbg_ctr] = 'W';
                    i2c_dbg_ctr++;
#endif
                }
                if (i2c_data_rcv() == ADDR_I2C_BIT)
                {
#ifdef I2C_DEBUG
                    i2c_debug_flow[i2c_dbg_ctr] = 'A';
                    i2c_dbg_ctr++;
#endif
                }
                if (i2c_data_rcv() == DATA_I2C_BIT)
                {
#ifdef I2C_DEBUG
                    i2c_debug_flow[i2c_dbg_ctr] = 'D';
                    i2c_dbg_ctr++;
#endif
                }
                //*/
#endif

#ifdef I2C_DEBUG
                i2c_debug_flow[i2c_dbg_ctr] = reg_add;
                i2c_dbg_ctr++;
#endif
                if (i2c_data_rcv() == DATA_I2C_BIT)
                {
                    //write part...
#ifdef I2C_DEBUG
                    i2c_debug_flow[i2c_dbg_ctr] = 'd';
                    i2c_dbg_ctr++;
#endif
                    if (reg < 0xFF)
                    {
#ifdef I2C_DEBUG
                        i2c_debug_flow[i2c_dbg_ctr] = 'e';
                        i2c_dbg_ctr++;
#endif
                        while (PIR1bits.SSPIF == 0);
                        read_databyte(reg);
                    } else
                    {
#ifdef I2C_DEBUG
                        i2c_debug_flow[i2c_dbg_ctr] = 'f';
                        i2c_dbg_ctr++;
#endif
                        reg = verify_word_address(reg_add);
                        if (reg < 0xFF)
                        {
#ifdef I2C_DEBUG
                            i2c_debug_flow[i2c_dbg_ctr] = 'g';
                            i2c_dbg_ctr++;
#endif
                            while (PIR1bits.SSPIF == 0);
                            read_dataword(reg);
                        } else
                        {
#ifdef I2C_DEBUG
                            i2c_debug_flow[i2c_dbg_ctr] = 'h';
                            i2c_dbg_ctr++;
#endif
                            garbage = SSPBUF;

                        }
                    }
                } else
                {
                    //read part
#ifdef I2C_DEBUG
                    i2c_debug_flow[i2c_dbg_ctr] = 'm';
                    i2c_dbg_ctr++;
#endif
                    i2c_add = SSPBUF;
                    //if (i2c_data_rcv() == ADDR_I2C_BIT) {
                    //i2c_debug_flow[i 2c_dbg_ctr]='n';
                    //i2c_dbg_ctr++;
                    if (i2c_add == SLAVE_ADDRESS << 1 | 1)
                    {
#ifdef I2C_DEBUG
                        i2c_debug_flow[i2c_dbg_ctr] = 'o';
                        i2c_dbg_ctr++;
#endif
                        if (i2c_bit_info() == READ_I2C_BIT_INFO)
                        {
#ifdef I2C_DEBUG
                            i2c_debug_flow[i2c_dbg_ctr] = 'p';
                            i2c_dbg_ctr++;
#endif
                            //reg=SSPBUF;
                            //SSPCON1bits.CKP = 1;
                            //PIR1bits.SSPIF = 0;
                            //while (PIR1bits.SSPIF == 0);
#ifdef I2C_DEBUG
                            i2c_debug_flow[i2c_dbg_ctr] = reg_add;
                            i2c_dbg_ctr++;
#endif
                            if (reg < 0xFF)
                            {
#ifdef I2C_DEBUG
                                i2c_debug_flow[i2c_dbg_ctr] = 'q';
                                i2c_dbg_ctr++;
#endif
                                //SSPBUF = 0XBF;
                                SSPBUF = i2c_read_handler(reg);
                                SSPCON1bits.CKP = 1;
                                PIR1bits.SSPIF = 0;
                            } else
                            {
#ifdef WORD_READ
                                reg = verify_word_address(reg_add);
                                if (reg < 0xFF)
                                {
                                    //non yet implemented.
                                    SSPBUF = i2c_read_handler_word(reg);
                                    SSPCON1bits.CKP = 1;
                                    PIR1bits.SSPIF = 0;
                                    SSPBUF = i2c_read_handler_word(reg);
                                    SSPCON1bits.CKP = 1;
                                    PIR1bits.SSPIF = 0;
                                } else
                                {
#endif
#ifdef I2C_DEBUG
                                    i2c_debug_flow[i2c_dbg_ctr] = 'r';
                                    i2c_dbg_ctr++;
#endif
                                    SSPBUF = 0xFF;
                                    SSPCON1bits.CKP = 1;
                                    PIR1bits.SSPIF = 0;
#ifdef WORD_RED
                                }
#endif
                            }
                        }
                    }
                    //}
                }
            } else
            {
                if (i2c_bit_info() == READ_I2C_BIT_INFO)
                {
                    while (PIR1bits.SSPIF == 0);
                    reg_add = SSPBUF;
                    reg = verify_byte_address(reg_add);
                    if (reg < 0xFF)
                    {
                        SSPCON1bits.CKP = 1;
                        PIR1bits.SSPIF = 0;
                        SSPBUF = i2c_read_handler(reg);
                        //write_databyte(reg);
                    }
                }
            }

        }
    }
#ifdef I2C_DEBUG
    i2c_debug_flow[i2c_dbg_ctr] = 'z';
    i2c_dbg_ctr++;
#endif
    SSPCON1bits.CKP = 1; //after i2c operation, set SCL to low
    PIR1bits.SSPIF = 0;
    i2c_start_flag = 0;
    wtimer = 0;
    wtimer_prescale = 0;
    wtimer_flag == 1;
    //*/
}


//#define DEBUG_MODE
//#define STARTUP             //enables startup blinking for reset detection leaves LED at High
#define INTERRUPT_ON
#define TIMER0_ON
#define I2C_COMMANDS      //enabled i2c receive commands
#define MOTION
#define SENSOR
#define SHOCK           0
#define LDR_IR          1
#define DOOR_DET
#define AUTO_ON
#define WATCHDOG
#define KEYPAD          1
//#define I2C_DEBUG
#ifdef I2C_DEBUG
#endif
//#define Interrupt_check     //toggles LED to check if(a condition of) intterupt is being executed
//#define IR_TO_LED_TEST      //toggles LED when PIC recieves verified IR DATA



/*copy this code to the part of interrupt for checking

#ifdef Interrupt_check
if (prescaler > 1000)
{
    PORTAbits.RA4 = ~PORTAbits.RA4;
    prescaler = 0;
} else
{
    prescaler++;
}
#endif

//*/
int debug;
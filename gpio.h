//SENSORS
#define TRIS_MOTION         TRISDbits.TRISD0
#define GPIO_MOTION         PORTDbits.RD0
#define ANSEL_MOTION        ANSELDbits.ANSD0

#define TRIS_SENSOR0        TRISAbits.TRISA0
#define GPIO_SENSOR0        PORTAbits.RA0
#define ANSEL_SENSOR0       ANSELAbits.ANSA0

#define TRIS_SENSOR1        TRISAbits.TRISA1
#define GPIO_SENSOR1        PORTAbits.RA1
#define ANSEL_SENSOR1       ANSELAbits.ANSA1

//RELAYS
#define TRIS_RLY0           TRISAbits.TRISA2
#define GPIO_RLY0           PORTAbits.RA2
#define ANSEL_RLY0          ANSELAbits.ANSA2

#define TRIS_RLY1           TRISAbits.TRISA3
#define GPIO_RLY1           PORTAbits.RA3
#define ANSEL_RLY1          ANSELAbits.ANSA3

//LED
#define TRIS_LED_OL         TRISAbits.TRISA4
#define GPIO_LED_OL         PORTAbits.RA4
#define ANSEL_LED_OL        ANSELAbits.ANSA4

#define TRIS_LED_LOCK       TRISAbits.TRISA5
#define GPIO_LED_LOCK       PORTAbits.RA5
#define ANSEL_LED_LOCK      ANSELAbits.ANSA5

#define TRIS_LED_UNLOCK     TRISAbits.TRISA6
#define GPIO_LED_UNLOCK     PORTAbits.RA6
#define ANSEL_LED_UNLOCK    ANSELAbits.ANSA6

#define TRIS_LED_FLASH_EN1     TRISAbits.TRISA7
#define GPIO_LED_FLASH_EN1     PORTAbits.RA7
#define ANSEL_LED_FLASH_EN1    ANSELAbits.ANSA7

#define TRIS_LED_FLASH_EN2     TRISEbits.TRISE0
#define GPIO_LED_FLASH_EN2     PORTEbits.RE0
#define ANSEL_LED_FLASH_EN2    ANSELEbits.ANSE0

#define TRIS_LED_LIGHT      TRISDbits.TRISD1
#define GPIO_LED_LIGHT      PORTDbits.RD1
#define ANSEL_LED_LIGHT     ANSELAbits.ANSA2

#define TRIS_PHONE_BUTTON      TRISEbits.TRISE1
#define GPIO_PHONE_BUTTON      PORTEbits.RE1
#define ANSEL_PHONE_BUTTON     ANSELEbits.ANSE1

//AUTO_ON
#define TRIS_ON             TRISDbits.TRISD2
#define GPIO_ON             PORTDbits.RD2
#define ANSEL_ON            ANSELDbits.ANSD2

//AUDIO SWITCH
//AUDIOAMP /AMP
#define TRIS_AUDIO_AMP      TRISBbits.TRISB2
#define GPIO_AUDIO_AMP      PORTBbits.RB2
#define ANSEL_AUDIO_AMP     ANSELBbits.ANSB2
//MIC-AMP /SW
#define TRIS_MIC_AMP        TRISBbits.TRISB3
#define GPIO_MIC_AMP        PORTBbits.RB3
#define ANSEL_MIC_AMP       ANSELBbits.ANSB3
//HANDSET /MOSFET
#define TRIS_HANDSET        TRISBbits.TRISB4
#define GPIO_HANDSET        PORTBbits.RB4
#define ANSEL_HANDSET       ANSELBbits.ANSB4

//ADC
#define LDR_TRIS    TRISBbits.TRISB4
#define LDR_GPIO    PORTBbits.RB4
#define LDR_ANSEL   ANSELBbits.ANSB4
#define LDR_CNL     11

//IRLED
#define IR1_TRIS    TRISAbits.TRISA4
#define IR1_GPIO    PORTAbits.RA4
#define IR1_ANSEL   ANSELAbits.ANSA4

#define IR2_TRIS    TRISAbits.TRISA5
#define IR2_GPIO    PORTAbits.RA5
#define IR2_ANSEL   ANSELAbits.ANSA5

#define IR3_TRIS    TRISAbits.TRISA6
#define IR3_GPIO    PORTAbits.RA6
#define IR3_ANSEL   ANSELAbits.ANSA6


//OBSOLETE
#define TRIS_DOOR           TRISAbits.TRISA4
#define GPIO_DOOR           PORTAbits.RA4

#define TRIS_SHOCK          TRISDbits.TRISD4
#define GPIO_SHOCK          PORTDbits.RD4
#define ANSEL_SHOCK         ANSELDbits.ANSD4

//KEYPAD COLUMN
#define TRIS_COLUMN1            TRISBbits.TRISB0
#define COLUMN1                 PORTBbits.RB0
#define ANSEL_COLUMN1           ANSELBbits.ANSB0
#define LAT_COLUMN1             LATBbits.LATB0

#define TRIS_COLUMN2            TRISBbits.TRISB1
#define COLUMN2                 PORTBbits.RB1
#define ANSEL_COLUMN2           ANSELBbits.ANSB1
#define LAT_COLUMN2             LATBbits.LATB1

#define TRIS_COLUMN3            TRISBbits.TRISB5
#define COLUMN3                 PORTBbits.RB5
#define ANSEL_COLUMN3           ANSELBbits.ANSB5
#define LAT_COLUMN3             LATBbits.LATB5

//KEYPAD ROW
#define TRIS_ROW1               TRISAbits.TRISA7
#define ROW1                    PORTAbits.RA7
//#define ANSEL_ROW1              ANSELAbits.ANSA7
#define LAT_ROW1                LATAbits.LATA7

#define TRIS_ROW2               TRISEbits.TRISE0
#define ROW2                    PORTEbits.RE0
#define ANSEL_ROW2              ANSELEbits.ANSE0
#define LAT_ROW2                LATEbits.LATE0

#define TRIS_ROW3               TRISEbits.TRISE1
#define ROW3                    PORTEbits.RE1
#define ANSEL_ROW3              ANSELEbits.ANSE1
#define LAT_ROW3                LATEbits.LATE1

#define TRIS_ROW4               TRISEbits.TRISE2
#define ROW4                    PORTEbits.RE2
#define ANSEL_ROW4              ANSELEbits.ANSE2
#define LAT_ROW4                LATEbits.LATE2



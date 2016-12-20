int ldr_en=0;
int ir_en=0;
int sensor0_en = 0;
int sensor1_en = 0;
int motion_en = 0;
int adc_flag = 0;
int relay0_en = 0;
int relay1_en = 0;
int keypad_en = 0;

int ir_level=0;
int ldr_value=0;
int sensor0_status = 0;
int sensor1_status = 0;


void init_sensor0();
void init_sensor1();
void init_motion();

void deinit_sensor0();
void deinit_sensor1();
void deinit_motion();

void init_relay1();
void init_relay2();
void init_led_light();
void init_led_flash_en1();
void init_led_flash_en2();
void init_led_ol();
void init_led_lock();
void init_led_unlock();
void init_auto_on();
void init_led_phone_button();

void init_audio_switch();

void init_ldr();
void deinit_ldr();

void init_ir();
void deinit_ir();
void ir_level_service();

int shock_detect();
int motion_detect();
int sensor1_detect();
int sensor0_detect();
int door_detect();
void adc_read_service();

#define LDR_READ        1
#define LDR_READING     2
#define LDR_READ_DONE   3
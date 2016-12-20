#define SHOCK_ACTIVE        0
#define SHOCK_INACTIVE      1
#define SHOCK_SENSITIVITY   20

#define MOTION_ACTIVE       0
#define MOTION_INACTIVE     1
#define MOTION_SENSITIVITY  20

#define SENSOR_ACTIVE       0
#define SENSOR_INACTIVE     1
#define SENSOR_SENSITIVITY  20

#define DOOR_LOCK           1
#define DOOR_UNLOCK         0

#define LED_ON              1
#define LED_OFF             0

#define RLY_ACTIVE          1
#define RLY_INACTIVE        0

#define ON_ACTIVE           1
#define ON_INACTIVE         0

int motion_sensitivity  = MOTION_SENSITIVITY;
int sensor0_sensitivity  = SENSOR_SENSITIVITY;
int sensor1_sensitivity  = SENSOR_SENSITIVITY;

int ldr_sensitivity1 = 30;
int ldr_sensitivity2 = 50;
int ldr_sensitivity3 = 70;
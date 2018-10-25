
#include "mbed.h"
#include "pin_mapping.h"
#include "temperature.h"

I2C temp_sensor(TEMP_SDA_PIN, TEMP_SCL_PIN);

void temp_init()
{
    
}

float temp_measure()
{
    static float i = 0;
    i += 1.1f;
    if(i >= 100) i = 0;
    return i;
}


#include "mbed.h"
#include "pin_mapping.h"
#include "temperature.h"

#define TEMP_SENSOR_I2C_ADDRESS (0x90)
#define TEMP_SENSOR_CMD_START (0x51)
#define TEMP_SENSOR_CMD_READ (0xAA)

I2C temp_sensor(TEMP_SDA_PIN, TEMP_SCL_PIN);

void temp_sensor_write_cmd(char cmd)
{
    temp_sensor.write(TEMP_SENSOR_I2C_ADDRESS, &cmd, sizeof(cmd), false);
}

void temp_sensor_init()
{
    temp_sensor_write_cmd(TEMP_SENSOR_CMD_START);
}

float temp_measure()
{
    char data[2];
    uint16_t data_combined;
    
    temp_sensor_write_cmd(TEMP_SENSOR_CMD_READ);
    temp_sensor.read(TEMP_SENSOR_I2C_ADDRESS, data, 2, false);
    
    data_combined = (data[0] << 8) | data[1];
    return (data_combined >> 4) / 16.0f;
}

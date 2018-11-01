/**
\file temperature.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>

Temperature sensor control used for Project 2 Module 3 <br>
Temperature sensor used: https://datasheets.maximintegrated.com/en/ds/DS1631-DS1731.pdf <br>

Designed by: Chris Morroni and Ranger Beguelin <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

#include "mbed.h"
#include "pin_mapping.h"
#include "temperature.h"

///I2C address of the temperature sensor is 0x90 (as defined in the datasheet)
#define TEMP_SENSOR_I2C_ADDRESS (0x90)
///Temperature sensor start command is 0x51 (as defined in the datasheet)
#define TEMP_SENSOR_CMD_START (0x51)
///Temperature sensor read command is 0xAA (as defined in the datasheet)
#define TEMP_SENSOR_CMD_READ (0xAA)

///declare an I2C object with the SCL and SDA lines as defined in "pin_mapping.h"
I2C temp_sensor(TEMP_SDA_PIN, TEMP_SCL_PIN);

/**
* Name: temp_sensor_write_cmd <br>
* Description: this function writes the user specified command to the temperature sensor <br>
* @param [in] char cmd
* @param [out] none
*/
void temp_sensor_write_cmd(char cmd)
{
    temp_sensor.write(TEMP_SENSOR_I2C_ADDRESS, &cmd, sizeof(cmd), false);
}

/**
* Name: temp_sensor_init <br>
* Description: this function initializes the temperature sensor using "temp_sensor_write_cmd()"<br>
* and the start command defined at the top of the program <br>
* @param [in] none
* @param [out] none
*/
void temp_sensor_init()
{
    temp_sensor_write_cmd(TEMP_SENSOR_CMD_START);
}

/**
* Name: temp_measure <br>
* Description: this function gets a temperature measurement from the sensor <br>
* and then returns it in degrees celsius <br>
* @param [in] none
* @param [out] float
*/
float temp_measure()
{
    char data[2];
    uint16_t data_combined;
    
    temp_sensor_write_cmd(TEMP_SENSOR_CMD_READ); //write the command to the sensor to take a measurement
    temp_sensor.read(TEMP_SENSOR_I2C_ADDRESS, data, 2, false); //read the temperature data, storing the two bytes returned in the array "data"
    
    data_combined = (data[0] << 8) | data[1]; //combined the data into one integer by shifting the most significant bits and filling in the bits they vacate with the least significant bits
    return (data_combined >> 4) / 16.0f;
}

#include "mbed.h"
#include "pin_mapping.h"
#include "temperature.h"

//#define 
const int temp_sensor_address = 0x90; //address of the temperature sensor on the I2C bus. This is defined by the chip

I2C temp_sensor(TEMP_SDA_PIN, TEMP_SCL_PIN); //I2C object labelled "temp_sensor"

void temp_init() //function to initialize the temperature sensor with the appropriate settings
{
    char start = 0x51; //data to send to the sensor to initiate temperature conversions
    char* data; //the write function needs a pointer to the data you want to send, instead of the data itself
    data = &start;// assign the pointer to the memory address of the data you want to send
    //temp_sensor.frequency (100000); //set the frequency of the I2C bus
    
    //format: write(int address, const char *data, int length, bool repeated=false)
    temp_sensor.write(temp_sensor_address, data, 1, false); //send the command to start
    //temp_sensor.start(); 
    //temp_sensor.write(temp_sensor_address);
    //temp_sensor.write(start);
    //temp_sensor.stop();
    //wait(2);
    //temp_measure();

}

float temp_measure() //function to actual execute the temperature measurement
{
    static float i; //float value to return and print
    char start = 0xAA; //data to send to the sensor to read temperatures
    char* data; //the write function needs a pointer to the data you want to send, instead of the data itself
    data = &start; //assign the pointer to the memory address of the data you want to send
    char data_read[2]; //an empty array that the sensor will fill with data during a "read" command
    int16_t output; //16 bit integer that will be given the value contained in both elements of the data_read array
    
    //format: read(int address, char *data, int length, bool repeated=false)
    temp_sensor.write(temp_sensor_address, data, 1, false); //"read temperature command" according to the datasheet
    temp_sensor.read(temp_sensor_address, data_read, 2, false); //read actual sensor data
    
    //temp_sensor.write(temp_sensor_address, data, 1); //"read temperature command" according to the datasheet
    //temp_sensor.read(temp_sensor_address, data_read, 2); //read actual sensor data
  
    output = data_read[0]; //to start, set output equal to the most signifant byte of the temperature data
    output = output << 8; //shift output left by 8, putting the contents of data_read[1] into the most significant bits of output
    output |= data_read[1]; //bitwise OR the contents of data_read[0] to output in order to set the least significant byte of output equal to data_read[0]
    
    output = output >> 4; //shift output to the right by 4 to clear out the "dont cares" that occupy the first 4 bits of output
    i = output / 16.0f;
    return i;
}
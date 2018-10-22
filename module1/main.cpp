#include "mbed.h"
#include "dhry.h"

DigitalOut myled(LED1);
Serial pc(SERIAL_TX, SERIAL_RX);

int main() {
    
    dhry_main(1000000);
    
    pc.printf("RTC example\r\n"); 
    set_time(1539639581); // Set RTC time to 16 December 2013 10:05:23 UTC
    pc.printf("Date and time are set.\r\n");

    while(1) {

        time_t seconds = time(NULL);

        //printf("Time as seconds since January 1, 1970 = %d\n", seconds);
        
        pc.printf("Time as a basic string = %s\r\n", ctime(&seconds));

        //char buffer[32];
        //strftime(buffer, 32, "%I:%M:%S %p\n", localtime(&seconds));
        //printf("Time as a custom formatted string = %s", buffer);

        myled = !myled;      
        wait(1);
    }
}

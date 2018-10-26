
#include "mbed.h"
#include "pin_mapping.h"
#include "uart.h"

#define UART_BAUD (115200)

Serial pc(USB_TX_PIN, USB_RX_PIN);

void uart_init()
{
    pc.baud(UART_BAUD);
}

void uart_printf(const char * format, ... )
{
    va_list args;
    va_start(args, format);
    pc.vprintf(format, args);
    va_end(args);
}
#ifndef UART_H
#define UART_H

#include <stddef.h>
#include <stdint.h>

extern void uartprint(char* s);
extern void uartprinthex(char* s, size_t len);

// UART device.
typedef struct uart uart;

// reads at most `cap` bytes from the UART device into `buf`, returning the
// number of bytes read.
size_t uartread(uart* dev, uint8_t* buf, size_t cap);

#endif

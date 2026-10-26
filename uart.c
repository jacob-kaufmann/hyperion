#include "uart.h"

// UART register map.
typedef struct regmap {
  volatile uint8_t rbrthr_dll;
  volatile uint8_t ier_dlm;
  volatile uint8_t iirfcr;
  volatile uint8_t lcr;
  volatile uint8_t mcr;
  volatile uint8_t lsr;
  volatile uint8_t msr;
  volatile uint8_t scr;
} regmap;

struct uart {
  volatile regmap regs;
};

size_t uartread(uart* dev, uint8_t* buf, size_t cap) {
  // DLAB bit of the LCR must be logic low (i.e. zero) to acess the RBR.
  volatile uint8_t lcr = dev->regs.lcr;
  lcr &= 0b01111111;
  dev->regs.lcr = lcr;

  // DR (i.e. data ready) bit of the LSR.
  volatile uint8_t is_data_ready = dev->regs.lsr;
  is_data_ready &= 0b00000001;

  // while data is available and there is remaining capacity, read a character
  // from the UART device receive buffer into the given buffer.
  size_t i = 0;
  while (is_data_ready && i < cap) {
    buf[i] = dev->regs.rbrthr_dll;
    is_data_ready = dev->regs.lsr;
    is_data_ready &= 0b00000001;
    i += 1;
  }

  return i;
}

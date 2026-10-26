#include "platform.h"

// universal asynchronous receiver-transmitter (UART).
const uintptr_t MMIO_ADDR_UART = 0x10000000;
// platform-level interrupt controller (PLIC).
const uintptr_t MMIO_ADDR_PLIC = 0x0c000000;
// `mtime` register.
const uintptr_t MMIO_ADDR_MTIME = 0x0200bff8;
// base address of `mtimecmp` register(s).
const uintptr_t MMIO_ADDR_MTIMECMP_BASE = 0x02004000;

const int plicctx(size_t const hart, privilege const priv) {
  switch (priv) {
    case PRIVILEGE_M:
      return 2 * hart;
    case PRIVILEGE_S:
      return 2 * hart + 1;
    case PRIVILEGE_U:
      return -1;
  }
}

const uintptr_t mmio_addr_mtimecmp(size_t const hart) {
  return MMIO_ADDR_MTIMECMP_BASE + (hart * 8);
}

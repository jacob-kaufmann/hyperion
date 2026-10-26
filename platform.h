#ifndef PLATFORM_H
#define PLATFORM_H

#include <stddef.h>
#include <stdint.h>

// RISC-V privilege mode. on RISC-V, there are three privilege modes: machine,
// supervisor, and user.
typedef enum privilege {
  PRIVILEGE_M,
  PRIVILEGE_S,
  PRIVILEGE_U,
} privilege;

// returns the PLIC context corresponding to hart ID `hart` and privilege mode
// `priv`. returns `-1` on error.
const int plicctx(size_t const hart, privilege const priv);

// returns the address of the `mtimecmp` memory-mapped register for `hart`.
const uintptr_t mmio_addr_mtimecmp(size_t const hart);

// memory-mapped I/O addresses.
// 
// NOTE: typically, we would determine these addresses via the device tree blob
// (DTB), but for QEMU virt, the addresses are fixed.
//
// universal asynchronous receiver transmitter (UART) device.
extern const uintptr_t MMIO_ADDR_UART;
// platform-level interrupt controller (PLIC).
extern const uintptr_t MMIO_ADDR_PLIC;
// `mtime` register.
extern const uintptr_t MMIO_ADDR_MTIME;
// base of `mtimecmp` register(s).
extern const uintptr_t MMIO_ADDR_MTIMECMP_BASE;

#endif

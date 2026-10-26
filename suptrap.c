#include "suptrap.h"

#include "platform.h"
#include "uart.h"

#include <stdbool.h>
#include <stdint.h>

extern void sup_ecall_mtimecmp_set(uint64_t);

// S mode trap handler.
void sup_traphandler(trapframe* const tf) {
  // the highest bit of `scause` is a flag that is set if the cause of the trap
  // was an interrupt.
  bool is_interrupt = tf->xcause >> ((sizeof(size_t)) - 1);
  size_t cause = (tf->xcause << 1) >> 1;

  uartprint("sepc: ");
  uartprinthex((char*) &tf->xepc, sizeof(size_t));
  uartprint("\n");
  uartprint("scause: ");
  uartprinthex((char*) &tf->xcause, sizeof(size_t));
  uartprint("\n");
  uartprint("stval: ");
  uartprinthex((char*) &tf->xtval, sizeof(size_t));
  uartprint("\n");

  // TODO: check for fatal or non-fatal?
  // 
  // TODO: profile against lookup table (i.e. LUT) and against vectored trap
  // mode as opposed to direct trap mode in `stvec`.
  if (is_interrupt) {
    sup_inthandler(tf, (trapint) cause);
  } else {
    sup_exchandler(tf, (trapexc) cause);
  }
}

// S mode interrupt handler.
static void sup_inthandler(trapframe* const tf, trapint const cause) {
  switch (cause) {
    case TRAPINT_SUP_TIMER:
      sup_inthandler_timer();
      break;
    case TRAPINT_SUP_EXT:
      sup_inthandler_ext(tf);
      break;
    case TRAPINT_SUP_SOFTWARE:
    case TRAPINT_COUNT_OVERFLOW:
    default:
      uartprint("unimplemented interrupt...\n");
      break;
  }
}

// timer interrupt handler. because we do not assume the Sstc extension is
// enabled, a supervisor timer interrupt occurs after a machine timer interrupt
// handler sets the STIP bit of the `mip` CSR. then, this timer interrupt
// handler can handle the interrupt and subsequently trap to machine mode via
// `ecall` to increment `mtimecmp`, arming the next timer interrupt.
static void sup_inthandler_timer() {
  // NOTE: we assume that we can read `mtime` in supervisor mode based on the
  // `mcounteren` CSR.
  uint64_t volatile * time = (uint64_t volatile *) MMIO_ADDR_MTIME;
  uint64_t now = *time;

  // compute the time of the next tick.
  const uint64_t TICK = 10000000;
  uint64_t next = now + TICK;

  sup_ecall_mtimecmp_set(next);
}

// handles external interrupts.
//
// to service an interrupt, we transmit an interrupt claim to the PLIC in the
// form of a memory-mapped I/O register read. then, the PLIC atomically
// determines the ID of the highest priority pending interrupt for the interrupt
// target and clears the corresponding interrupt sources's interrupt pending
// bit. the PLIC returns the ID if there was a pending interrupt and zero
// otherwise. since there might be other (lower priority) pending interrupts,
// the PLIC external interrupt pending bit may not be clear after a claim. after
// we service the interrupt, we transmit an interrupt completion to the PLIC in
// the form of a memory-mapped I/O register write with the same ID returned by
// the PLIC.
static void sup_inthandler_ext(trapframe* const tf) {
  uint32_t volatile * reg = ((uint32_t*) (MMIO_ADDR_PLIC + ((uintptr_t) 0x201004)));
  volatile uint32_t id = *reg;
  if (id == PLIC_INTSRC_UART) {
    // the interrupt source is the UART device.
    // 
    // TODO: check that the interrupt corresponds to received data ready or a
    // recevied data timeout (if in FIFO mode).
    uart* dev = (uart*) MMIO_ADDR_UART;
    uint8_t buf[64] = {0};
    size_t n = uartread(dev, buf, 64);
    uartprinthex((char*) buf, n);
  }
  *reg = id;
}

// S mode exception handler.
static void sup_exchandler(trapframe* const tf, trapexc const cause) {
  switch (cause) {
    case TRAPEXC_ILL_INSTRUCTION:
    case TRAPEXC_BREAKPOINT:
    case TRAPEXC_ENV_CALL_USER:
    case TRAPEXC_ENV_CALL_SUP:
    default:
      uartprint("unimplemented exception...\n");
      break;
  }

  // increment the program counter to the next instruction. RISC-V instructions
  // are fixed-width instructions.
  // 
  // TODO: is there a situation where we do not want to increment the PC here?
  const uint64_t ILEN = 4;
  tf->xepc += ILEN;
}

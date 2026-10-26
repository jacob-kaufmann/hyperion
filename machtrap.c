#include "machtrap.h"

#include "platform.h"
#include "trap.h"
#include "uart.h"

#include <stdbool.h>

extern void mach_inthandler_timer();
extern void sup_ecallhandler_mtimecmp_set(uint64_t mtimecmp);

void mach_traphandler(trapframe* const tf) {
  // the highest bit of `scause` is a flag that is set if the cause of the trap
  // was an interrupt.
  bool is_interrupt = tf->xcause >> ((sizeof(size_t)) - 1);
  size_t cause = (tf->xcause << 1) >> 1;

  uartprint("mepc: ");
  uartprinthex((char*) &tf->xepc, sizeof(size_t));
  uartprint("\n");
  uartprint("mcause: ");
  uartprinthex((char*) &tf->xcause, sizeof(size_t));
  uartprint("\n");
  uartprint("mtval: ");
  uartprinthex((char*) &tf->xtval, sizeof(size_t));
  uartprint("\n");

  // TODO: check for fatal or non-fatal?
  // 
  // TODO: profile against lookup table (i.e. LUT) and against vectored trap
  // mode as opposed to direct trap mode in `stvec`.
  if (is_interrupt) {
    mach_inthandler(tf, (trapint) cause);
  } else {
    mach_exchandler(tf, (trapexc) cause);
  }
}

// M mode interrupt handler.
static void mach_inthandler(trapframe* const tf, trapint const cause) {
  uartprint("handling interrupt...\n");
  switch (cause) {
    case TRAPINT_MACH_TIMER:
      mach_inthandler_timer();
      break;
    case TRAPINT_MACH_EXT:
      mach_inthandler_ext(tf);
      break;
    case TRAPINT_MACH_SOFTWARE:
    case TRAPINT_COUNT_OVERFLOW:
    default:
      // TODO: add info to print statement.
      uartprint("unimplemented interrupt...\n");
      break;
  }
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
static void mach_inthandler_ext(trapframe* const tf) {
  uint32_t* reg = ((uint32_t*) (MMIO_ADDR_PLIC + ((uintptr_t) 0x201004)));
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

// M mode exception handler.
static void mach_exchandler(trapframe* const tf, trapexc const cause) {
  uartprint("handling exception...\n");
  switch (cause) {
    case TRAPEXC_ENV_CALL_SUP:
      mach_exchandler_ecall_sup(tf);
      break;
    case TRAPEXC_ILL_INSTRUCTION:
    case TRAPEXC_BREAKPOINT:
    case TRAPEXC_ENV_CALL_USER:
    default:
      // TODO: add info to print statement.
      // TODO: fatal?
      uartprint("unimplemented exception...\n");
      break;
  }

  // increment the program counter to the next instruction. RISC-V instructions
  // are fixed-width instructions.
  // 
  // TODO: is there a situation where we do not want to increment the PC here?
  tf->xepc += 4;
}

static void mach_exchandler_ecall_sup(trapframe* const tf) {
  // register `a7` of the trap frame `tf` holds the function code.
  // 
  // TODO: what if cast fails?
  switch ((ecall) tf->x17) {
    case ECALL_MTIMECMP_SET:
      sup_ecallhandler_mtimecmp_set(tf->x10);
      break;
    default:
      uartprint("unimplemented ecall...\n");
      break;
  }
}

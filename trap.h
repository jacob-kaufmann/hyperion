#ifndef TRAP_H
#define TRAP_H

#include <stddef.h>

// trap frame.
typedef struct trapframe {
  size_t x1;
  size_t x2;
  size_t x3;
  size_t x4;
  size_t x5;
  size_t x6;
  size_t x7;
  size_t x8;
  size_t x9;
  size_t x10;
  size_t x11;
  size_t x12;
  size_t x13;
  size_t x14;
  size_t x15;
  size_t x16;
  size_t x17;
  size_t x18;
  size_t x19;
  size_t x20;
  size_t x21;
  size_t x22;
  size_t x23;
  size_t x24;
  size_t x25;
  size_t x26;
  size_t x27;
  size_t x28;
  size_t x29;
  size_t x30;
  size_t x31;
  size_t xepc;
  size_t xcause;
  size_t xtval;
  size_t xstatus;
} trapframe;

// PLIC interrupt source IDs.
typedef enum plic_int_src {
  // UART device.
  PLIC_INTSRC_UART = 10,
} plic_intsrc;

// interrupt exception codes.
typedef enum trapint {
  // supervisor software.
  TRAPINT_SUP_SOFTWARE = 1,
  // machine software.
  TRAPINT_MACH_SOFTWARE = 3,
  // supervisor timer.
  TRAPINT_SUP_TIMER = 5,
  // machine timer.
  TRAPINT_MACH_TIMER = 7,
  // supervisor external.
  TRAPINT_SUP_EXT = 9,
  // machine external.
  TRAPINT_MACH_EXT = 11,
  // supervisor counter-overflow.
  TRAPINT_COUNT_OVERFLOW = 13,
} trapint;

const char* trapint_to_string(const trapint cause);

// exception exception codes.
typedef enum trapexc {
  // illegal instruction.
  TRAPEXC_ILL_INSTRUCTION = 2,
  // breakpoint.
  TRAPEXC_BREAKPOINT = 3,
  // environment call from U mode.
  TRAPEXC_ENV_CALL_USER = 8,
  // environment call from S mode.
  TRAPEXC_ENV_CALL_SUP = 9,
} trapexc;

const char* trapexc_to_string(const trapexc cause);

// `ecall` function identifiers.
typedef enum ecall {
  ECALL_MTIMECMP_SET = 0,
} ecall;

#endif

#ifndef MACHTRAP_H
#define MACHTRAP_H

#include "trap.h"

void mach_traphandler(trapframe* const tf);

static void mach_inthandler(trapframe* const tf, trapint const cause);
static void mach_inthandler_ext(trapframe* const tf);

static void mach_exchandler(trapframe* const tf, trapexc const cause);
static void mach_exchandler_ecall_sup(trapframe* const tf);

#endif

#ifndef SUPTRAP_H
#define SUPTRAP_H

#include "trap.h"

void sup_traphandler(trapframe* const tf);

static void sup_inthandler(trapframe* const tf, trapint const cause);
static void sup_inthandler_timer();
static void sup_inthandler_ext(trapframe* const tf);

static void sup_exchandler(trapframe* const tf, trapexc const cause);

#endif

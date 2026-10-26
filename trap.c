#include "trap.h"

const char* trapint_to_string(const trapint cause) {
  switch (cause) {
    case TRAPINT_SUP_SOFTWARE :
      return "supervisor software";
    case TRAPINT_MACH_SOFTWARE:
      return "machine software";
    case TRAPINT_SUP_TIMER :
      return "supervisor timer";
    case TRAPINT_MACH_TIMER:
      return "machine timer";
    case TRAPINT_SUP_EXT :
      return "supervisor external";
    case TRAPINT_MACH_EXT:
      return "machine external";
    case TRAPINT_COUNT_OVERFLOW :
      return "counter overflow";
    default:
      return "unimplemented exception";
  }
}

const char* trapexc_to_string(const trapexc cause) {
  switch (cause) {
    case TRAPEXC_ILL_INSTRUCTION:
      return "illegal instruction";
    case TRAPEXC_BREAKPOINT:
      return "breakpoint";
    case TRAPEXC_ENV_CALL_USER:
      return "environment call from user mode";
    case TRAPEXC_ENV_CALL_SUP:
      return "environment call from supervisor mode";
    default:
      return "unimplemented exception";
  }
}

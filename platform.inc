# QEMU virt platform-specific constants.

# entry points.
.equ ENTRYPOINT_BOOTLOADER, 0x80000000
.equ ENTRYPOINT_KERNEL,     0x80200000

# memory-mapped register addresses.
.equ MMIO_ADDR_MTIME,         0x0200bff8
.equ MMIO_ADDR_MTIMECMP_BASE, 0x02004000

################################################################################
# universal asynchronous receiver transmitter (UART) device constants.
################################################################################

# the base address of the 16550 UART device.
.equ UART, 0x10000000

# UART device register.
.equ UART_OFFSET_LCR, 3
.equ UART_OFFSET_IER, 1
.equ UART_MASK_LCR_DLAB,  0b10000000
.equ UART_MASK_IER_ERBFI, 0b00000001

################################################################################
# platform level interrupt controller (PLIC) platform constants.
################################################################################

# the base address of the memory-mapped PLIC.
.equ PLIC, 0x0c000000

# the offset of the interrupt source priorities from the PLIC base address.
.equ PLIC_OFFSET_INT_PRIORITY, 0x0

# the offset of the interrupt source enable bits from the PLIC base address.
.equ PLIC_OFFSET_INT_SRC_ENABLE, 0x2000

# the offset of the interrupt priority thresholds from the PLIC base address.
.equ PLIC_OFFSET_INT_THRESHOLD, 0x200000

# the PLIC context corresponding to HART 0 and privilege S.
.equ PLIC_CTX_KERNEL, 1

# the PLIC interrupt source corresponding to the UART device.
.equ PLIC_INT_SRC_UART, 10

# the PLIC interrupt priority of the UART device.
.equ PLIC_INT_PRIORITY_UART, 1

################################################################################
# PLIC memory map.
################################################################################
#
# NOTE: the PLIC memory map is specific to the RISC-V ISA, not QEMU virt.
# TODO: move to `riscv.inc`.
#
# the base address of the memory map is platform-specific. offsets from the
# base address are given for each register block in the memory map.
#
# PLIC interrupt source priority memory map.
# 0x000000: reserved (interrupt source 0 does not exist)
# 0x000004: interrupt source 1 priority
# 0x000008: interrupt source 2 priority
# ...
#
# PLIC interrupt source pending memory map.
# 0x001000: interrupt source 0-31 pending bits
# ...
# 0x00107c: interrupt source 992-1023 pending bits
#
# PLIC interrupt enable memory map.
# 0x002000: interrupt source 0-31 enable bits on context 0
# ...
# 0x00207c: interrupt source 992-1023 enable bits on context 0
# 0x002080: interrupt source 0-31 enable bits on context 1
# ...
# 0x0020fc: interrupt source 992-1023 enable bits on context 1
# 0x002100: interrupt source 0-31 enable bits on context 2
# ...
# 0x00217c: interrupt source 992-1023 enable bits on context 2
# ...
# 
# PLIC interrupt priority threshold memory map.
# 0x200000: interrupt priority threshold for context 0
# 0x201000: interrupt priority threshold for context 1
# 0x202000: interrupt priority threshold for context 2
# ...
#
# PLIC interrupt claim/completion memory map.
# 0x200004: interrupt claim/completion for context 0
# 0x201004: interrupt claim/completion for context 1
# 0x202004: interrupt claim/completion for context 2
# ...
################################################################################
#
################################################################################

# facilities to write (i.e. print) to the UART serial device.

.include "platform.inc"

.section .text
.balign 16

# assume a0 holds the address of the null-terminated string to print.
.global uartprint
uartprint:
  # load the base address of the UART serial device into a temporary register.
  li t1, UART

1:
  # load the next byte of the output string. if we are at the end of the string
  # (i.e. null terminator), then return. otherwise, load the next byte of the
  # string into a temporary register.
  lbu  t2, 0(a0)
  beqz t2, 3f
2:
  # load the status line register of the UART device, and check whether the
  # transmitter is empty. if it is empty, then we can write the byte and
  # increment to the subsequent byte. otherwise, loop back, and check again.
  #
  # TODO: add symbolic constants for the transmitter offset and mask.
  lbu  t3, 5(t1)
  andi t3, t3, 0x20
  beqz t3, 2b
  sb   t2, 0(t1)
  addi a0, a0, 1
  j    1b
3:
  ret

# assume a0 holds the address of the byte array to print.
# assume a1 holds the length (in bytes) of the byte array.
.global uartprinthex
uartprinthex:
  # load the base address of the UART serial device into a temporary register.
  li t0, UART

  # load an index into a temporary register.
  mv t1, zero

  li t6, 9

# if we are at the end of the byte array (i.e. index equals length), then
# return. otherwise, load the next byte of the byte array into a temporary
# register.
1:
  # t1 holds the index, t2 holds the address of the current byte, and t3 holds
  # the current byte.
  #
  # TODO: load an entire register worth of bytes to reduce memory traffic.
  beq  t1, a1, 8f
  add  t2, a0, t1
  lbu  t3, 0(t2)
# load the status line register of the UART device, and check whether the
# transmitter is empty. if it is empty, then we can write the byte and
# increment to the subsequent byte. otherwise, loop back, and check again.
#
# to write the byte as hex (i.e. two hex symbols), we need to isolate the
# lower four bits and the upper four bits, map those "nibbles" to hex ASCII
# symbols, and write the corresponding bytes to the device.
#
# TODO: add symbolic constants for the transmitter offset and mask.
2:
  # high nibble.
  lbu  t4, 5(t0)
  andi t4, t4, 0x20
  beqz t4, 2b
  andi t5, t3, 0xf0
  srli t5, t5, 4
  bgt  t5, t6, 3f

  # numeric
  addi t5, t5, 48
  j    4f
3:
  # alphabetical
  addi t5, t5, 87
4:
  sb   t5, 0(t0)
5:
  # low nibble.
  lbu  t4, 5(t0)
  andi t4, t4, 0x20
  beqz t4, 5b
  andi t5, t3, 0x0f
  bgt  t5, t6, 6f

  # numeric
  addi t5, t5, 48
  j    7f
6:
  # alphabetical
  addi t5, t5, 87
7:
  sb   t5, 0(t0)

  # increment the loop index and jump to the top of the loop.
  addi t1, t1, 1
  j    1b
8:
  # TODO: print new line?
  ret


ENTRY(bootentry)

/* NOTE: QEMU `virt` maps DRAM to 0x80000000. */

SECTIONS
{
  . = 0x80000000;
  /* NOTE: we explicity mark `.text.entry` so that the entry point is located
     at the address above. `KEEP` prevents garbage collection from discarding
     `bootentry` even if there are no references to it.
  */
  .text    : { KEEP(*(.text.entry)), *(.text .text.*) }
  .rodata  : { *(.rodata .rodata.*) }
  .bss     : { *(.bss .bss.*) }
}

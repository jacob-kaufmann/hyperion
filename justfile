target := "riscv64-unknown-elf"

# `zicsr` extension is necessary for control and status registers.
arch := "rv64ima_zicsr"

# integer calling-convention only (no hardware floating-point).
abi := "lp64"

# do not include the C standard library.
# 
# do not include the C start files (e.g. crt), which execute before `main`. we
# do not need those start files since we initialize the stack, zero `bss`, etc.
#
# use LLVM linker `lld`.
#
# TODO: add DWARF debug information?
# TODO: is -g flag necessary here?
clanglinkopts := "-nostdlib -nostartfiles -fuse-ld=lld -g"

# debug compilation options.
debugopts := "-g -O0"

build-libs:
    # UART assembly facilities.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          {{debugopts}} \
          -c uart.s \
          -o uartasm.o
    # UART facilities.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          -fno-stack-protector \
          -ffreestanding \
          {{clanglinkopts}} \
          {{debugopts}} \
          -c uart.c \
          -o uart.o
    # platform facilities.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          -fno-stack-protector \
          -ffreestanding \
          {{clanglinkopts}} \
          {{debugopts}} \
          -c platform.c \
          -o platform.o
      
build-bootloader: build-libs
    # machine trap assembly facilities.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          {{debugopts}} \
          -c machtrap.s \
          -o machtrapasm.o
    # machine trap facilities.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          -fno-stack-protector \
          -ffreestanding \
          {{clanglinkopts}} \
          {{debugopts}} \
          -c machtrap.c \
          -o machtrap.o
    # bootloader.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          {{debugopts}} \
          -c bootloader.s \
          -o bootloader.o

link-bootloader: build-bootloader
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          {{clanglinkopts}} \
          -Wl,-T,bootloader.ld uartasm.o uart.o platform.o machtrapasm.o machtrap.o bootloader.o \
          -o bootloader.elf

build-kernel: build-libs
    # kernel trap assembly facilities.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          {{debugopts}} \
          -c suptrap.s \
          -o suptrapasm.o
    # kernel trap facilities.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          -fno-stack-protector \
          -ffreestanding \
          {{clanglinkopts}} \
          {{debugopts}} \
          -c suptrap.c \
          -o suptrap.o
    # kernel.
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          -mcmodel=medany \
          {{debugopts}} \
          -c kernel.s \
          -o kernel.o

link-kernel: build-kernel
    clang --target={{target}} \
          -march={{arch}} \
          -mabi={{abi}} \
          {{clanglinkopts}} \
          -Wl,-T,kernel.ld uartasm.o uart.o platform.o suptrapasm.o suptrap.o kernel.o \
          -o kernel.elf

# debug with a port open for remote GDB/LLDB, where the program is frozen at
# startup.
debug gdb-port: link-bootloader link-kernel
    llvm-objcopy -O binary kernel.elf kernel.bin
    qemu-system-riscv64 -machine virt \
                        -bios none \
                        -display none \
                        -serial stdio \
                        -gdb tcp::{{gdb-port}} \
                        -S \
                        -kernel bootloader.elf \
                        -device loader,file=kernel.bin,addr=0x80200000
    
# first, we copy the kernel ELF as a binary, so that we can load that directly
# into memory. a production bootloader would expect a raw binary, not an ELF,
# since there would not be anything available in the ROM to parse an ELF.
run: link-bootloader link-kernel
    llvm-objcopy -O binary kernel.elf kernel.bin
    qemu-system-riscv64 -machine virt \
                        -bios none \
                        -display none \
                        -monitor none \
                        -serial stdio \
                        -kernel bootloader.elf \
                        -device loader,file=kernel.bin,addr=0x80200000

# clean up object files, ELF files, binary files, etc.
clean:
    rm *.o
    rm *.elf
    rm *.bin

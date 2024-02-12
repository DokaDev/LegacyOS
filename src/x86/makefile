GCC32 = gcc -c -m32 -ffreestanding
LD32 = ld -melf_i386 -T ./elf_i386.ld -nostdlib -e Main -Ttext 0x10200
OBJCOPY32 = objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJPATH = tmp
all: entry cdeps binary elf_bin kernelBin

entry: ./EntryPoint.s
	mkdir -p $(OBJPATH)
	nasm -o $(OBJPATH)/entryPoint.bin $^

cdeps:
	$(GCC32) -MM $(wildcard ./*.c) > $(OBJPATH)/deps.d

binary:
	make -C $(OBJPATH)

elf_bin: $(OBJPATH)/bin32.elf
	$(OBJCOPY32) $^ $(OBJPATH)/kernel.bin

kernelBin: $(OBJPATH)/kernel.bin $(OBJPATH)/entryPoint.bin
	cat $^ > ../../build/kernel32.bin
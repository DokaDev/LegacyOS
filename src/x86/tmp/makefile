GCC32 = x86_64-pc-linux-gnu-gcc -c -m32 -ffreestanding
LD32 = x86_64-pc-linux-gnu-ld -melf_i386 -T ../elf_i386.ld -nostdlib -e Main -Ttext 0x10200

%.o: ../%.c
	$(GCC32) $<

%.o: ../%.asm
	nasm -f elf32 -o $@ $<

bin32.elf: Main.o $(subst Main.o, , $(notdir $(patsubst %.c, %.o, $(wildcard ../*.c)))) $(notdir $(patsubst %.asm, %.o, $(wildcard ../*.asm)))
	$(LD32) -o $@ $^

ifeq (deps.d, $(wildcard ./deps.d))
include deps.d
endif
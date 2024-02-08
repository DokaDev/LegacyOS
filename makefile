all: clean kernel utility image

kernel:
	mkdir -p build
	make -C src

utility:
	make -C tools

image: build/boot.bin build/kernel32.bin
	# cat $^ > build/os.img


clean: 
	clear

	rm -f build/*.*

	rm -f src/boot/*.o
	rm -f src/boot/*.bin

	rm -f src/x86/tmp/*.o
	rm -f src/x86/tmp/*.bin
	rm -f src/x86/tmp/*.elf
	rm -f src/x86/tmp/*.d

	rm -f tools/ImageMaker/bin/*.*

test:
	qemu-system-x86_64 -m 64 -fda build/os.img -rtc base=localtime -M pc
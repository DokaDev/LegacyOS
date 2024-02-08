all: clean kernel utility image

kernel:
	mkdir -p build
	make -C src

utility:
	make -C tools

image: build/boot.bin build/kernel32.bin

	# cat $^ > build/os.img


clean: 
	rm -f build/*.*

	rm -f src/boot/*.o
	rm -f src/boot/*.bin

	rm -f src/x86/tmp/*.o
	rm -f src/x86/tmp/*.bin
	rm -f src/x86/tmp/*.elf

	rm -f tools/ImageMaker/bin/*.*

qemu:
	qemu
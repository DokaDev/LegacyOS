all: clean kernel

kernel:
	make -C src

clean: 
	rm -f build/*.*

	rm -f src/boot/*.o
	rm -f src/boot/*.bin

	
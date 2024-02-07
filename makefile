all: clean kernel

kernel:
	make -C src

clean: 
	rm -f ./build/*.*
BUILD_DIR = build
IMG = kernel8.img

all:
	make -C src
	cp -r src/build ./
clean:
	rm -rf build
	rm -rf src/build
run-debug:
	qemu-system-aarch64 -M raspi3\
		-kernel $(BUILD_DIR)/$(IMG)\
		-display none\
		-serial null\
		-serial stdio\
		-S -s

run:
	qemu-system-aarch64 -M raspi3\
		-kernel $(BUILD_DIR)/$(IMG)\
		-display none\
		-serial null\
		-initrd src/initramfs.cpio\
		-serial stdio 

gdb:
	$(GDB) $(GDBFLAGS) $(GDBFILE)



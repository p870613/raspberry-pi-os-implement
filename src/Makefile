CC = aarch64-linux-gnu-gcc
CCFLAGS = -Iinclude -Wall -ffreestanding  -nostdlib -nostartfiles -ggdb -Wno-unused-but-set-variable

LD = aarch64-linux-gnu-ld
LDFLAGS = -T

OBJCOPY = aarch64-linux-gnu-objcopy
OBJCOPYFLAGS = -O binary

GDB = gdb-multiarch
GDBFLAGS = -x 
GDBFILE = ./debug.gdb

BUILD_DIR = build
ELF = kernel.elf
SRC = $(wildcard **/*.S) $(wildcard **/*.c) $(wildcard **/mm/*.c)
IMG = kernel8.img
#OBJ = $(BUILD_DIR)/exception.o $(BUILD_DIR)/boot.o $(filter-out $(BUILD_DIR)/boot.o, $(BUILD_DIR)/exception.o, $(wildcard $(BUILD_DIR)/*.o))
OBJ = $(BUILD_DIR)/boot.o $(filter-out $(BUILD_DIR)/boot.o, $(wildcard $(BUILD_DIR)/*.o))
LINKER_SCRIPTS = $(wildcard **/*.ld)

all:
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CCFLAGS) -c $(SRC) 
	mv *.o $(BUILD_DIR)
	make kerne
kerne:
	$(LD) $(LDFLAGS) $(LINKER_SCRIPTS) -o $(BUILD_DIR)/$(ELF) $(OBJ)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(BUILD_DIR)/$(ELF) $(BUILD_DIR)/$(IMG)

run-debug:
	qemu-system-aarch64 -M raspi3\
		-kernel $(BUILD_DIR)/$(IMG)\
		-display none\
		-initrd initramfs.cpio\
		-serial null\
		-serial stdio\
		-S -s

run:
	qemu-system-aarch64 -M raspi3\
		-kernel $(BUILD_DIR)/$(IMG)\
		-display none\
		-serial null\
		-initrd initramfs.cpio\
		-serial stdio


run-ramdisk:
	qemu-system-aarch64 -M raspi3\
		-kernel $(BUILD_DIR)/$(IMG)\
		-display none\
		-serial null\
		-initrd initramfs.cpio\
		-serial stdio

gdb:
	$(GDB) $(GDBFLAGS) $(GDBFILE)

objdump:
	aarch64-linux-gnu-objdump -d ./build/kernel.elf | less 

clean:
	rm -rf build
	rm -rf *.o

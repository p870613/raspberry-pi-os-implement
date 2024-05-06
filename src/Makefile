CC = aarch64-linux-gnu-gcc
CFLAGS = -Iinclude -Wall -ffreestanding  -nostdlib -nostartfiles -ggdb -Wno-unused-but-set-variable
#CFLAGS = -Wall -ggdb -Og -nostdlib -nostartfiles -ffreestanding -Iinclude -Ilib -Iperipheral -Ikernel -Imm -Itest
LD = aarch64-linux-gnu-ld
LDFLAGS = -T

OBJCOPY = aarch64-linux-gnu-objcopy
OBJCOPYFLAGS = -O binary

GDB = gdb-multiarch
GDBFLAGS = -x 
GDBFILE = ./debug.gdb

BUILD_DIR = ./
HEADER := $(wildcard */*.h) $(wildcard */*/*.h)
ELF = $(BUILD_DIR)kernel.elf
SRC = $(wildcard **/*.c) $(wildcard */*/*.c)
IMG = $(BUILD_DIR)kernel8.img
ASM := $(wildcard **/*.S) $(wildcard */*/*.S)
OBJ = $(patsubst %.S, %.o, $(ASM)) $(patsubst %.c, %.o, $(SRC)) 
#OBJ = $(BUILD_DIR)/exception.o $(BUILD_DIR)/boot.o $(filter-out $(BUILD_DIR)/boot.o, $(BUILD_DIR)/exception.o, $(wildcard $(BUILD_DIR)/*.o))
#OBJ = $(BUILD_DIR)/boot.o $(filter-out $(BUILD_DIR)/boot.o, $(wildcard $(BUILD_DIR)/*.o))
LINKER_SCRIPTS = $(wildcard **/*.ld)

all: $(IMG) 
%o: %s %c
$(ELF): $(OBJ)
	echo $(ASM)
	$(LD) $(LDFLAGS) $(LINKER_SCRIPTS) -o $@ $(OBJ)

$(IMG): $(ELF)
	$(OBJCOPY) -O binary $< $@

run-debug:
	qemu-system-aarch64 -M raspi3\
		-kernel $(IMG)\
		-display none\
		-initrd initramfs.cpio\
		-serial null\
		-serial stdio\
		-S -s

run:
	qemu-system-aarch64 -M raspi3\
		-kernel $(IMG)\
		-display none\
		-serial null\
		-initrd initramfs.cpio\
		-serial stdio


run-ramdisk:
	qemu-system-aarch64 -M raspi3\
		-kernel $(IMG)\
		-display none\
		-serial null\
		-initrd initramfs.cpio\
		-serial stdio

gdb:
	$(GDB) $(GDBFLAGS) $(GDBFILE)

objdump:
	aarch64-linux-gnu-objdump -d $(ELF) | less 

clean:
	rm -rf build
	rm -rf $(wildcard */*.o) $(wildcard */*/*.o)
	rm -rf $(ELF)
	rm -rf $(IMG)

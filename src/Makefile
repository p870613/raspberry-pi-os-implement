CC = aarch64-linux-gnu-gcc
CCFLAGS = -Iinclude -Wall -ffreestanding  -nostdlib -nostartfiles -mgeneral-regs-only

LD = aarch64-linux-gnu-ld
LDFLAGS = -T

OBJCOPY = aarch64-linux-gnu-objcopy
OBJCOPYFLAGS = -O binary

GDB = aarch64-linux-gnu-gdb
GDBFLAGS = -x

BUILD_DIR = build
ELF = kernel.elf
SRC = $(wildcard **/*.S) $(wildcard **/*.c)
IMG = kernel8.img
OBJ = $(BUILD_DIR)/boot.o $(filter-out $(BUILD_DIR)/boot.o, $(wildcard $(BUILD_DIR)/*.o))
LINKER_SCRIPTS = $(wildcard **/*.ld)
GDBFILE = ./debug.gdb

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
		-serial null\
		-serial stdio\
		-S -s

run:
	qemu-system-aarch64 -M raspi3\
		-kernel $(BUILD_DIR)/$(IMG)\
		-display none\
		-serial null\
		-serial stdio

gdb:
	$(GDB) $(GDBFLAGS) $(GDBFILE)

clean:
	rm -rf build
	rm -rf *.o

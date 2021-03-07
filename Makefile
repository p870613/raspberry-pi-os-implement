CC= aarch64-linux-gnu-gcc
CXXFLAGS = -Iinclude -Wall -ffreestanding  -nostdlib -nostartfiles

LD = aarch64-linux-gnu-ld
LDFLAGS = -T

OBJCOPY = aarch64-linux-gnu-objcopy
OBJCOPYFLAGS = -O binary

GDB = aarch64-linux-gnu-gdb
GDBFLAGS = -x

BUILD_DIR = build
ELF = kernel.elf
SRC = $(wildcard **/*.S) $(wildcard **/*.cc)
IMG = kernel8.img
OBJ = boot.o $(filter-out boot.o $(wildcard *.o))
LINKER_SCRIPTS = $(wildcard **/*.ld)
GDBFILE = ./debug.gdb

all:
	mkdir -p $(BUILD_DIR) 
	$(CXX) $(CXXFLAGS) -c $(SRC)
	make kernel

kernel:
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(ELF) $(OBJ)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(BUILD_DIR)/$(ELF) $(BUILD_DIR)/$(IMG)



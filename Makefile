# ARM Cortex-M0 FreeRTOS Makefile
# Requirements: arm-none-eabi-gcc toolchain

# Project name
PROJECT = freertos_cm0

# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size

# Directories
SRC_DIR = src
INC_DIR = include
FREERTOS_DIR = FreeRTOS/Source
BUILD_DIR = build

# CPU and FPU settings
CPU = -mcpu=cortex-m0
THUMB = -mthumb

# C flags
CFLAGS = $(CPU) $(THUMB) -std=c99 -Wall -Wextra
CFLAGS += -fdata-sections -ffunction-sections -fno-strict-aliasing -fno-builtin
CFLAGS += -Os -g3

# Assembly flags  
ASFLAGS = $(CPU) $(THUMB) -x assembler-with-cpp

# Linker flags
LDFLAGS = $(CPU) $(THUMB)
LDFLAGS += -specs=nano.specs -specs=nosys.specs
LDFLAGS += -T cortex-m0.ld
LDFLAGS += -Wl,--gc-sections -Wl,--print-memory-usage
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map

# Include directories
INCLUDES = -I$(INC_DIR)
INCLUDES += -I$(FREERTOS_DIR)/include
INCLUDES += -I$(FREERTOS_DIR)/portable/GCC/ARM_CM0

# Source files
C_SOURCES = $(SRC_DIR)/main.c
C_SOURCES += $(FREERTOS_DIR)/tasks.c
C_SOURCES += $(FREERTOS_DIR)/queue.c
C_SOURCES += $(FREERTOS_DIR)/list.c
C_SOURCES += $(FREERTOS_DIR)/timers.c
C_SOURCES += $(FREERTOS_DIR)/event_groups.c
C_SOURCES += $(FREERTOS_DIR)/stream_buffer.c
C_SOURCES += $(FREERTOS_DIR)/portable/GCC/ARM_CM0/port.c
C_SOURCES += $(FREERTOS_DIR)/portable/MemMang/heap_4.c

# Assembly sources
ASM_SOURCES = $(SRC_DIR)/startup.s

# Object files
C_OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS = $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

# Default target
all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).lst
	@echo "Build complete!"
	$(SIZE) $(BUILD_DIR)/$(PROJECT).elf

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	@mkdir -p $(BUILD_DIR)/$(FREERTOS_DIR)
	@mkdir -p $(BUILD_DIR)/$(FREERTOS_DIR)/portable/GCC/ARM_CM0
	@mkdir -p $(BUILD_DIR)/$(FREERTOS_DIR)/portable/MemMang

# ELF file
$(BUILD_DIR)/$(PROJECT).elf: $(BUILD_DIR) $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

# HEX file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@

# LST file (disassembly)
$(BUILD_DIR)/$(PROJECT).lst: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJDUMP) -h -S $< > $@

# Compile C sources
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

# Compile assembly sources
$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) -c $(ASFLAGS) $(INCLUDES) $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Show help
help:
	@echo "Available targets:"
	@echo "  all    - Build project (default)"
	@echo "  clean  - Clean build files"
	@echo "  help   - Show this help"

.PHONY: all clean help
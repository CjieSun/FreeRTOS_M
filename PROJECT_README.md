# FreeRTOS ARM Cortex-M0 Project

This project implements a FreeRTOS-based system for ARM Cortex-M0 microcontrollers with a simple multi-task demonstration.

## Features

- **Target CPU**: ARM Cortex-M0
- **Compiler**: arm-none-eabi-gcc
- **RTOS**: FreeRTOS v10.4.6
- **Memory Layout**:
  - Flash: 0x00000000 - 0x00080000 (512KB)
  - SRAM: 0x20000000 - 0x20010000 (64KB)
- **Scheduler**: 1ms tick rate as required
- **Demo**: Two tasks running at different priorities and frequencies

## Project Structure

```
├── src/
│   ├── main.c       # Main application with multi-task demo
│   └── startup.s    # ARM Cortex-M0 startup assembly
├── include/
│   └── FreeRTOSConfig.h  # FreeRTOS configuration
├── FreeRTOS/
│   └── Source/      # FreeRTOS kernel source files
├── cortex-m0.ld     # Linker script for specified memory layout
├── Makefile         # Build configuration
└── build/           # Generated build artifacts
```

## Multi-Task Demo

The demo includes two tasks:

1. **Task 1** (Lower priority): Runs every 1000ms (1 second)
   - Simulates LED1 toggle by incrementing `led1_counter`
   
2. **Task 2** (Higher priority): Runs every 500ms
   - Simulates LED2 toggle by incrementing `led2_counter`

## Building

### Prerequisites
- arm-none-eabi-gcc toolchain installed

### Build Commands
```bash
# Build the project
make all

# Clean build files
make clean

# Show help
make help
```

### Generated Files
After building, the following files are generated in the `build/` directory:
- `freertos_cm0.elf` - Executable and Linkable Format file
- `freertos_cm0.hex` - Intel HEX file for programming
- `freertos_cm0.lst` - Disassembly listing file
- `freertos_cm0.map` - Memory map file

## Memory Usage

Current build uses:
- Flash: ~5.7KB (1.09% of 512KB)
- RAM: ~17.9KB (27.26% of 64KB)

## Configuration

### FreeRTOS Configuration
- Tick rate: 1000 Hz (1ms tick)
- CPU clock: 48 MHz (assumed)
- Heap: 8KB (heap_4 memory management)
- Priority levels: 5
- Stack overflow detection enabled

### Memory Layout
The linker script defines the exact memory regions as specified:
- Flash starts at 0x00000000 (512KB)
- SRAM starts at 0x20000000 (64KB)
- Stack pointer initialized to 0x20010000

## Features Included

✓ ARM Cortex-M0 compatible code  
✓ FreeRTOS kernel v10.4.6  
✓ Multi-task demonstration  
✓ 1ms tick scheduling  
✓ Proper memory layout  
✓ Generates ELF, HEX, and LST files  
✓ Uses arm-none-eabi-gcc toolchain  
✓ No peripheral dependencies  

## Hook Functions

The following FreeRTOS hook functions are implemented:
- `vApplicationMallocFailedHook()` - Called when malloc fails
- `vApplicationStackOverflowHook()` - Called on stack overflow detection
- `vApplicationIdleHook()` - Called during idle task execution
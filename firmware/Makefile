# target
TARGET = gd32vf103

# building variables
# debug build?
#DEBUG = 1
# optimization
#OPT = -O2 #-flto
OPT = -O3
#OPT = -Og

# Build path
BUILD_DIR = build

# path
CURR_PATH = $(shell pwd)
FIRMWARE_DIR := $(CURR_PATH)
FIRMWARE_LIB_DIR := $(CURR_PATH)/GD32VF103_Firmware_Library/Firmware
#TOOLCHAIN_PATH = $(CURR_PATH)/nuclei/riscv-gnu-toolchain-bin/
#TOOLCHAIN_PATH = $(CURR_PATH)/nuclei/nuclei_riscv_newlibc_prebuilt_linux64_2022.04/bin/
TOOLCHAIN_PATH = $(CURR_PATH)/nuclei/nuclei_riscv_newlibc_prebuilt_linux64_2020.08/bin/
#PREFIX = $(TOOLCHAIN_PATH)/riscv64-unknown-elf-
PREFIX = $(TOOLCHAIN_PATH)/riscv-nuclei-elf-
OPENOCD_DIR = $(CURR_PATH)/nuclei/nuclei-openocd/0.10.0-15/bin
#DFU_DIR =


# firmware library
#PERIFLIB_SOURCES = \
# $(wildcard Lib/*.a)

# binaries
CC = $(PREFIX)gcc
#AS = $(PREFIX)gcc -x assembler-with-cpp
AS = $(PREFIX)gcc
CP = $(PREFIX)objcopy
AR = $(PREFIX)ar
SZ = $(PREFIX)size
OD = $(PREFIX)objdump
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


# source
# C sources
C_SOURCES = \
$(wildcard $(FIRMWARE_LIB_DIR)/GD32VF103_standard_peripheral/Source/*.c) \
$(wildcard $(FIRMWARE_LIB_DIR)/RISCV/drivers/*.c) \
$(wildcard $(FIRMWARE_LIB_DIR)/RISCV/env_Eclipse/*.c) \
$(wildcard $(FIRMWARE_LIB_DIR)/RISCV/stubs/*.c) \

C_SOURCES += \
$(wildcard $(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/device/core/Source/*.c) \
$(wildcard $(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/driver/Source/drv_usb_core.c) \
$(wildcard $(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/driver/Source/drv_usb_dev.c) \
$(wildcard $(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/driver/Source/drv_usbd_int.c) \
$(wildcard $(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/device/class/dfu/Source/*.c)

C_SOURCES += \
$(wildcard $(FIRMWARE_DIR)/src/*.c)

# ASM sources
ASM_SOURCES = \
$(wildcard $(FIRMWARE_LIB_DIR)/RISCV/env_Eclipse/*.S)


# C includes
C_INCLUDES = \
-I$(FIRMWARE_LIB_DIR)/GD32VF103_standard_peripheral/Include \
-I$(FIRMWARE_LIB_DIR)/RISCV/drivers \
-I$(FIRMWARE_LIB_DIR)/RISCV/stubs

C_INCLUDES += \
-I$(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/device/core/Include \
-I$(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/driver/Include \
-I$(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/ustd/common \
-I$(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/ustd/class/cdc \
-I$(FIRMWARE_LIB_DIR)/GD32VF103_usbfs_library/device/class/dfu/Include

C_INCLUDES += \
-I$(FIRMWARE_DIR)/inc

# AS includes
#AS_INCLUDES =

# macros for gcc
# AS defines
#AS_DEFS =

# C defines
#C_DEFS = \
#-DUSE_STDPERIPH_DRIVER
#-DHXTAL_VALUE=24000000U


# CFLAGS
# cpu
ARCH = -march=rv32imac -mabi=ilp32 -mcmodel=medany
#ARCH += -mcmodel=medlow


CFLAGS := $(ARCH) $(C_DEFS) $(C_INCLUDES) $(OPT)
CFLAGS += -Wl,-Bstatic
CFLAGS += -ffunction-sections -fdata-sections
#CFLAGS += -Wl,-ffreestanding
#CFLAGS += -nostdlib
#CFLAGS += -fsigned-char
#CFLAGS += -fno-common
CFLAGS += -std=gnu11
#CFLAGS += -std=gnu2x

ifeq ($(DEBUG), 1)
	CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP #.deps/$(notdir $(<:.c=.d)) -MF$(@:%.o=%.d) -MT$(@:%.o=%.d)


# compile gcc flags
ASFLAGS := $(ARCH) $(C_DEFS) $(C_INCLUDES) $(OPT)
#ASFLAGS += -c
ASFLAGS += -Wl,-Bstatic
#ASFLAGS += -Wl,-ffreestanding
#ASFLAGS += -nostdlib
ASFLAGS += -x assembler-with-cpp


# LDFLAGS
# link script
LDSCRIPT = $(FIRMWARE_LIB_DIR)/RISCV/env_Eclipse/GD32VF103xB.lds

# libraries
#LIBS = -lc_nano -lm
#LIBDIR =
#LDFLAGS = $(OPT) $(ARCH) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) $(PERIFLIB_SOURCES) \
 -Wl,--cref -Wl,--no-relax -Wl,--gc-sections -Wl,-M=$(BUILD_DIR)/$(TARGET).map -nostartfiles #-ffreestanding -nostdlib

LDFLAGS := $(ARCH) $(OPT)
LDFLAGS += -T$(LDSCRIPT)
#LDFLAGS += -lc_nano
#LDFLAGS += -lm
#LDFLAGS += -lc
#LDFLAGS += -lgcc
#LDFLAGS += -Wall
LDFLAGS += -Wl,--cref
LDFLAGS += -Wl,--no-relax
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map,$(BUILD_DIR)/$(TARGET).map
LDFLAGS += -nostartfiles
#LDFLAGS += -ffreestanding
#LDFLAGS += -nostdlib
#LDFLAGS += --specs=nano.specs
#LDFLAGS += -Xlinker
#LDFLAGS += --gc-sections



# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


# build the application
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) .deps
	@echo "CC $<"
	@$(CC) -c $(CFLAGS) -MMD -MP \
		-MF .deps/$(notdir $(<:.c=.d)) \
		-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR) .deps
	@echo "AS $<"
	@$(AS) -c $(ASFLAGS) -MMD -MP  \
		-MF .deps/$(notdir $(<:.S=.d)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo "LD $@"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "OD $@"
	@$(OD) $(BUILD_DIR)/$(TARGET).elf -xS > $(BUILD_DIR)/$(TARGET).S $@
	@echo "SIZE $@"
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "OBJCOPY $@"
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "OBJCOPY $@"
	@$(BIN) $< $@	

$(BUILD_DIR):
	mkdir $@

.deps:
	mkdir $@


# clean up
clean:
	-rm -fr .deps $(BUILD_DIR)

flash: all
	sudo $(OPENOCD_DIR)/openocd -f ../gd32vf103cbt6-old.cfg -c init -c "flash protect 0 0 last off; \
			program {$(BUILD_DIR)/$(TARGET).elf} verify; mww 0xe004200c 0x4b5a6978; mww 0xe0042008 0x01; resume; exit 0;"

dfu: all
#	sudo $(DFU_DIR)/dfu-util -d 28e9:0189 -a 0 -s 0x08000000:leave -D $(BUILD_DIR)/$(TARGET).bin
	sudo dfu-util -d 28e9:0189 -a 0 -s 0x08000000:leave -D $(BUILD_DIR)/$(TARGET).bin
	#Remove ':leave', the microcontroller will not boot to application when finished programming.

dfu-app: all
	sudo $(DFU_DIR)/dfu-util -d 28e9:0189 -a 0 -s 0x08010000:leave -D $(BUILD_DIR)/$(TARGET).bin

uart: all
	stm32flash -w $(BUILD_DIR)/$(TARGET).bin /dev/ttyACM0


# dependencies
-include $(shell mkdir .deps 2>/dev/null) $(wildcard .deps/*)
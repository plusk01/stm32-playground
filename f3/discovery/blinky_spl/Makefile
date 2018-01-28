# Change this to whatever project name you want
PROJECT = blinky

# Add your sources that you want compiled here
PROJECT_SRCS = main.c system_stm32f30x.c

###############################################################################

BOARD_DIR = ../..

# Debugger optons, must be empty or GDB
DEBUG ?= GDB

# Flash size (KB). Some low-end chips actually have more flash than advertised, use this to override.
# STM32F30xC has 256 (http://www.st.com/content/st_com/en/products/microcontrollers/stm32-32-bit-arm-cortex-mcus/stm32-mainstream-mcus/stm32f3-series/stm32f303.html?querycriteria=productId=LN1531)
FLASH_SIZE ?= 256

# Working directories
ROOT         = $(dir $(lastword $(MAKEFILE_LIST)))
SRC_DIR      = $(ROOT)
BUILD        = $(ROOT)/build
OBJECT_DIR   = $(BUILD)/obj
BIN_DIR      = $(BUILD)/bin
LIB_DIR      = $(BOARD_DIR)/lib
CMSIS_DIR    = $(LIB_DIR)/CMSIS
INCLUDE_DIRS = $(LIB_DIR) $(BOARD_DIR) $(SRC_DIR)
LINKER_DIR   = $(BOARD_DIR)

# Standard Peripheral Driver Library Sources
STDPERIPH_DIR	= $(LIB_DIR)/STM32F30x_StdPeriph_Driver
STDPERIPH_SRC   = $(notdir $(wildcard $(STDPERIPH_DIR)/src/*.c))
EXCLUDES	    = 
STDPERIPH_SRC  := $(filter-out ${EXCLUDES}, $(STDPERIPH_SRC))

# If you want to use StdPeriph, you must define -DUSE_STDPERIPH_DRIVER:
STDPERIPH_FLAGS = -DUSE_STDPERIPH_DRIVER

INCLUDE_DIRS := $(INCLUDE_DIRS) \
		   $(STDPERIPH_DIR)/inc \
		   $(USBFS_DIR)/inc \
		   $(CMSIS_DIR)/CM1/CoreSupport \
		   $(CMSIS_DIR)/CM1/DeviceSupport/ST/STM32F30x

LD_SCRIPT    = $(LINKER_DIR)/stm32_flash_f303_$(FLASH_SIZE)k.ld

ARCH_FLAGS   = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant -Wdouble-promotion
DEVICE_FLAGS = -DSTM32F303xC -DSTM32F303

ifneq ($(FLASH_SIZE),)
DEVICE_FLAGS := $(DEVICE_FLAGS) -DFLASH_SIZE=$(FLASH_SIZE)
endif


STM32F30x_COMMON_SRC = \
		   $(BOARD_DIR)/startup_stm32f30x.S


$(PROJECT)_CSRC = $(PROJECT_SRCS) \
		   $(STDPERIPH_SRC) \
		   $(STM32F30x_COMMON_SRC)


# Search path for sources
VPATH       := $(BOARD_DIR):$(SRC_DIR)
VPATH       := $(VPATH):$(CMSIS_DIR)/CM1/CoreSupport:$(CMSIS_DIR)/CM1/DeviceSupport/ST/STM32F30x
VPATH       := $(VPATH):$(STDPERIPH_DIR)/src

###############################################################################
# Things that might need changing to use different tools
#

# Find out if ccache is installed on the system
CCACHE := ccache
RESULT = $(shell (which $(CCACHE) > /dev/null 2>&1; echo $$?) )
ifneq ($(RESULT),0)
CCACHE :=
endif

# Tool names
CC          := arm-none-eabi-gcc
OBJCOPY     := arm-none-eabi-objcopy
SIZE        := arm-none-eabi-size

#
# Tool options.
#

ifeq ($(DEBUG),GDB)
OPTIMIZE     = -O0
LTO_FLAGS    = $(OPTIMIZE)
DEBUG_FLAGS  = -ggdb3 -DDEBUG
else
OPTIMIZE     = -Os
LTO_FLAGS    =  -flto -fuse-linker-plugin $(OPTIMIZE)
DEBUG_FLAGS  =
endif


CFLAGS       = $(ARCH_FLAGS) \
		   $(LTO_FLAGS) \
		   $(STDPERIPH_FLAGS) \
		   $(WARN_FLAGS) \
		   $(addprefix -I,$(INCLUDE_DIRS)) \
		   $(DEBUG_FLAGS) \
		   -Wall -Wpedantic -Wextra -Wunsafe-loop-optimizations -Wdouble-promotion -Wundef \
		   -ffunction-sections \
		   -fdata-sections \
		   $(DEVICE_FLAGS) \
		   -fverbose-asm -ffat-lto-objects \
		   -save-temps=obj \
		   -MMD -MP

ASFLAGS      = $(ARCH_FLAGS) \
		   $(WARN_FLAGS) \
		   -x assembler-with-cpp \
		   $(addprefix -I,$(INCLUDE_DIRS)) \
		  -MMD -MP

LDFLAGS      = -lm \
		   -nostartfiles \
		   --specs=nano.specs \
		   -lc \
		   -lnosys \
		   $(ARCH_FLAGS) \
		   $(LTO_FLAGS) \
		   $(WARN_FLAGS) \
		   $(DEBUG_FLAGS) \
		   -static \
		   -Wl,-gc-sections,-Map,$(TARGET_MAP) \
		   -Wl,-L$(LINKER_DIR) \
		   -Wl,--cref \
		   -T$(LD_SCRIPT)

# Things we will build

TARGET_BIN   = $(BIN_DIR)/$(PROJECT).bin
TARGET_HEX   = $(BIN_DIR)/$(PROJECT).hex
TARGET_ELF   = $(BIN_DIR)/$(PROJECT).elf

TARGET_OBJS  = $(addsuffix .o,$(addprefix $(OBJECT_DIR)/$(PROJECT)/,$(basename $($(PROJECT)_CSRC))))
TARGET_MAP   = $(OBJECT_DIR)/$(PROJECT).map


## Default make goal:
## hex         : Make filetype hex only
.DEFAULT_GOAL := bin

## Optional make goals:
## all         : Make all filetypes, binary and hex
all: hex bin

## binary      : Make binary filetype
## bin         : Alias of 'binary'
## hex         : Make hex filetype
bin:    $(TARGET_BIN)
binary: $(TARGET_BIN)
hex:    $(TARGET_HEX)

# List of buildable ELF files and their object dependencies.

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex --set-start 0x08000000 $< $@

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

$(TARGET_ELF):  $(TARGET_OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(SIZE) $(TARGET_ELF)

# Compile
$(OBJECT_DIR)/$(PROJECT)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(CFLAGS) $<

# Assemble
$(OBJECT_DIR)/$(PROJECT)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(ASFLAGS) $<

$(OBJECT_DIR)/$(PROJECT)/%.o: %.S
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(ASFLAGS) $<

clean:
	rm -rf build

flash: $(TARGET_BIN)
	dfu-util -a 0 -s 0x08000000:leave -D $(TARGET_BIN)

# rebuild everything when makefile changes
$(TARGET_OBJS) : Makefile

print-%:
	@echo '$*=$($*)'
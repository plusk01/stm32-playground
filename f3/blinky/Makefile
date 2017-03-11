# put your *.c source files here, make should handle the rest!
SRCS = blinky.c system_stm32f30x.c 

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME=blinky

# Location of the Libraries folder from the STM32F0xx Standard Peripheral Library
LIB_DIR=../lib/

# Location of the linker scripts
LDSCRIPT_INC=../

# location of OpenOCD Board .cfg files (only used with 'make program')
OPENOCD_BOARD_DIR=/home/matt/bin/openocd/share/openocd/scripts/board

# Configuration (cfg) file containing programming directives for OpenOCD
OPENOCD_PROC_FILE=../stm32f3-openocd.cfg

# that's it, no need to change anything below this line!

###################################################

CC		= arm-none-eabi-gcc
GDB 	= arm-none-eabi-gdb
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE 	= arm-none-eabi-size

CFLAGS  = -Wall -Wno-switch -g -std=c99 -Os  
CFLAGS += -mlittle-endian -mcpu=cortex-m4  -march=armv7e-m -mthumb
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffunction-sections -fdata-sections

LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map

###################################################

vpath %.a $(LIB_DIR)

ROOT=$(shell pwd)

CFLAGS += -DSTM32F303xC
#CFLAGS += -I $(LIB_DIR) 
CFLAGS += -I $(LIB_DIR)/CMSIS/CM1/DeviceSupport/ST/STM32F30x
CFLAGS += -I $(LIB_DIR)/CMSIS/CM1/CoreSupport
CFLAGS += -I $(LIB_DIR)/STM32F30x_StdPeriph_Driver/inc
CFLAGS += -I $(LIB_DIR)/STM32_USB-FS-Device_Driver/inc
CFLAGS += -include $(LIB_DIR)/STM32F30x_StdPeriph_Driver/stm32f30x_conf.h

STARTUP = Device/startup_stm32f30x.s # add startup file to build

# need if you want to build with -DUSE_CMSIS 
#SRCS += stm32f0_discovery.c
#SRCS += stm32f0_discovery.c stm32f0xx_it.c

OBJS = $(addprefix objs/,$(SRCS:.c=.o))
DEPS = $(addprefix deps/,$(SRCS:.c=.d))

###################################################

.PHONY: all lib proj program debug clean reallyclean

all: lib proj

-include $(DEPS)

lib:
	$(MAKE) -C $(LIB_DIR)

proj: 	$(PROJ_NAME).elf

dirs:
	mkdir -p deps objs

objs/%.o : %.c dirs
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

$(PROJ_NAME).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(STARTUP) -L$(LIB_DIR) -lstm32f3 -L$(LDSCRIPT_INC) -Tstm32f3.ld
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf

program: all
	openocd -f $(OPENOCD_BOARD_DIR)/stm32f3discovery.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/$(PROJ_NAME).bin" -c shutdown

debug: program
	$(GDB) -x extra/gdb_cmds $(PROJ_NAME).elf

clean:
	find ./ -name '*~' | xargs rm -f	
	rm -f objs/*.o
	rm -f deps/*.d
	rm -f dirs
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst

reallyclean: clean
	$(MAKE) -C $(LIB_DIR) clean

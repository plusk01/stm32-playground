# put your *.c source files here, make should handle the rest!
SRCS = blinky.c system_stm32f30x.c 

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME=blinky

# Root location of all board files
BOARD_DIR=..

# Location of the Libraries folder from the STM32F0xx Standard Peripheral Library
LIB_DIR=$(BOARD_DIR)/lib

# Location of the linker scripts
LDSCRIPT_INC=$(BOARD_DIR)

# Configuration (cfg) file containing programming directives for OpenOCD
OPENOCD_PROC_FILE=$(BOARD_DIR)/stm32f3-openocd.cfg

# that's it, no need to change anything below this line!

###################################################

CC		= arm-none-eabi-gcc
GDB 	= arm-none-eabi-gdb
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE 	= arm-none-eabi-size

# -mlittle-endian
CFLAGS  = -Wall -Wno-switch -std=c99 -O0 -ggdb3
CFLAGS += -mcpu=cortex-m4  -march=armv7e-m -mthumb 
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffunction-sections -fdata-sections

LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map

###################################################

# Help look for the CMSIS/StdPeriph_Driver archive/lib
vpath %.a $(LIB_DIR)

# Help look for the system_stm32f30x.c
vpath %.c $(BOARD_DIR)

CFLAGS += -DSTM32F303xC
CFLAGS += -I $(LIB_DIR)/CMSIS/CM1/DeviceSupport/ST/STM32F30x
CFLAGS += -I $(LIB_DIR)/CMSIS/CM1/CoreSupport
CFLAGS += -I $(LIB_DIR)/STM32F30x_StdPeriph_Driver/inc
CFLAGS += -I $(LIB_DIR)/STM32_USB-FS-Device_Driver/inc
CFLAGS += -include $(LIB_DIR)/stm32f30x_conf.h

# add startup file to build
STARTUP = $(BOARD_DIR)/startup_stm32f30x.S

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
	touch dirs

objs/%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

objs/%.o : %.c dirs
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

$(PROJ_NAME).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -L$(LIB_DIR) -lstm32f30x -L$(LDSCRIPT_INC) -Tstm32_flash_f303_256k.ld
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf

program: all
	openocd -f ../openocd.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/$(PROJ_NAME).bin"

debug: program
	$(GDB) -x extra/gdb_cmds $(PROJ_NAME).elf

clean:
	@find ./ -name '*~' | xargs rm -f	
	@rm -f objs/*.o
	@rm -f deps/*.d
	@rm -f dirs
	@rm -f $(PROJ_NAME).elf
	@rm -f $(PROJ_NAME).hex
	@rm -f $(PROJ_NAME).bin
	@rm -f $(PROJ_NAME).map
	@rm -f $(PROJ_NAME).lst

reallyclean: clean
	$(MAKE) -C $(LIB_DIR) clean

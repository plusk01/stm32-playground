CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c STM32F30x_StdPeriph_Driver/src

CFLAGS  = -g -O2 -Wall -DSTM32F303xC
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -march=armv7e-m
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -include stm32f30x_conf.h
CFLAGS += -ICMSIS/CM1/CoreSupport
CFLAGS += -ICMSIS/CM1/DeviceSupport/ST/STM32F30x
CFLAGS += -ISTM32F30x_StdPeriph_Driver/inc
#CFLAGS += -I../inc

SRCS = stm32f30x_adc.c stm32f30x_can.c stm32f30x_comp.c stm32f30x_crc.c \
	stm32f30x_dac.c stm32f30x_dbgmcu.c stm32f30x_dma.c stm32f30x_exti.c \
	stm32f30x_flash.c stm32f30x_gpio.c stm32f30x_hrtim.c stm32f30x_i2c.c \
	stm32f30x_iwdg.c stm32f30x_misc.c stm32f30x_opamp.c stm32f30x_pwr.c \
	stm32f30x_rcc.c stm32f30x_rtc.c stm32f30x_spi.c stm32f30x_syscfg.c \
	stm32f30x_tim.c stm32f30x_usart.c stm32f30x_wwdg.c

OBJS = $(addprefix objs/,$(SRCS:.c=.o))
DEPS = $(addprefix deps/,$(SRCS:.c=.d))

.PHONY: all clean dirs

all: libstm32f30x.a

-include $(DEPS)

dirs:
	mkdir -p deps objs

objs/%.o : %.c dirs
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

libstm32f30x.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f deps objs $(OBJS) $(DEPS) libstm32f0.a
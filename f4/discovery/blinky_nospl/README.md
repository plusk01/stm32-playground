Blinky No Standard Perhipheral Library (SPL)
============================================

This simple example uses the code from [this](http://fos.cmb.ac.lk/esl/tutorial-3-blinking-led-without-using-libraries/) tutorial. The purpose of this example is to show what C code would look like if it only used the ARM CMSIS support headers and none of the STMicroelectronics driver support library, known as the **Standard Peripheral Library (SPL)**. A somewhat lightweight Makefile is included.

```bash
# 1. Plug into the STM32F3 Discovery's 'USB User' port
# 2. Short the BOOT0 and GND pins
# 3. Press the reset button and notice how `dmesg -w` shows
#     Product: STM32 BOOTLOADER
$ make flash # uses dfu-util
```

Alternatively, you could use the STLink debugger included on the discovery board. To do this, use `openocd` with the proper configuration file.

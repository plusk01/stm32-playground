STM32 F3 Projects
=================

Here are example projects demonstrating various aspects of STM32 F3 processors.

## Flashing ##

To flash the processor, you need to boot the microcontroller into its *system memory*, where the STM32 bootloader lives. Note that because of this separation of memory, it is impossible to brick your microcontroller.

To boot from system memory, short the `BOOT0` pin to ground and restart the chip. If you open `dmesg -w` before you do this, you will see the following:

```bash
[1060744.113680] usb 5-2: new full-speed USB device number 36 using xhci_hcd
[1060744.314682] usb 5-2: New USB device found, idVendor=0483, idProduct=df11
[1060744.314687] usb 5-2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[1060744.314689] usb 5-2: Product: STM32  BOOTLOADER
[1060744.314692] usb 5-2: Manufacturer: STMicroelectronics
[1060744.314694] usb 5-2: SerialNumber: 2059324F2033
```

Once you have verified that the BOOTLOADER is running, you can use `dfu-utils` to flash your binary onto the microcontroller. This can be done as illustrated in prevalent Makefile target:


```Makefile
flash: $(TARGET_BIN)
    dfu-util -a 0 -s 0x08000000:leave -D $(TARGET_BIN)
```

## Using a Debugger ##

Alternatively, you can use a ST-Link debugger to program the microcontroller. To do this, you need to install `openocd` via:

```bash
sudo apt install openocd
```

Then, you need to point to the proper script that tells `openocd` how to talk to your specific programmer (find the scripts using `sudo updatedb && locate -i openocd`).

For simplicity, I like to create aliases that supply all the information needed. For example, I might have a bash alias for the STM32 F3 Discovery board that looks like:

```bash
alias openocdf3discovery='openocd -f /usr/share/openocd/scripts/board/stm32f3discovery.cfg'
```

## Resources ##

- [STM32F3 Discovery Template](https://github.com/mblythe86/stm32f3-discovery-basic-template)
- [Great minimalist tutorial/reference](http://www.triplespark.net/elec/pdev/arm/stm32.html)

---

## Common Struggles ##

#### Board won't enumerate through VCP port ####

If you have a micro with VCP capability (STM32F30x, etc) and your device isn't showing up in `lsusb`, you may want to check `dmesg -w`. Then, if you plug your device in and you see something like:

```bash
[41652.544029] usb 7-1.1: new full-speed USB device number 19 using ehci-pci
[41652.616024] usb 7-1.1: device descriptor read/64, error -32
[41652.792025] usb 7-1.1: device descriptor read/64, error -32
[41652.968034] usb 7-1.1: new full-speed USB device number 20 using ehci-pci
[41653.040027] usb 7-1.1: device descriptor read/64, error -32
[41653.216036] usb 7-1.1: device descriptor read/64, error -32
[41653.392029] usb 7-1.1: new full-speed USB device number 21 using ehci-pci
[41653.800031] usb 7-1.1: device not accepting address 21, error -32
[41653.872024] usb 7-1.1: new full-speed USB device number 22 using ehci-pci
[41654.280025] usb 7-1.1: device not accepting address 22, error -32
[41654.280163] usb 7-1-port1: unable to enumerate USB device
```

This means that the firmware on you device has not properly configured the VCP, so when your computer asks the device who/what it is, your STM32 board doesn't know how to respond. This can be overcome in two ways:

1. Use a debugger (like the STLink-V2 SWD Debugger) to program a `.bin` onto the board that has VCP support
1. Short the `BOOT` pins to load into the booloader, which will make the device show up in DFU mode through the VCP.

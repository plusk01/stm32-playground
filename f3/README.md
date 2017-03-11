https://github.com/mblythe86/stm32f3-discovery-basic-template

Great minimalist tutorial/reference: http://www.triplespark.net/elec/pdev/arm/stm32.html

-------------------------------------------------------------

## Board won't enumerate through VCP port ##

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

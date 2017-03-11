STM32 Playground
================

This repo is to help me get up to speed on (and remember) various facets of programming STM32 devices. My end goal is to contribute to STM32-based flight controllers and autopilots such as [ROSflight](https://github.com/byu-magicc/ROSflight). In particular, I will be developing on the [BeeCore F3](http://www.banggood.com/Beecore-F3_EVO_Brushed-ACRO-Flight-Control-Board-DSM2-For-Inductrix-Tiny-Whoop-Eachine-E010-p-1089021.html).

## Getting Started ##

A great place to start is with the `minimalist` directory. It is an independent STM32 project that uses no peripherals and no magic. Undertanding these basics wil help to gain an intuition on what all the drivers, libraries, `Makefile`s, and linker scripts are doing.

Then, to see the use of the `CMSIS` and `StdPeriph_Driver` libs in action, take a look at the `f3` folder.

## Relevant Flight Controller Boards ##
- [Naze32 / Flip32 (F103)](http://www.readytoflyquads.com/the-flip32)
- [BeeCore F3](http://www.banggood.com/Beecore-F3_EVO_Brushed-ACRO-Flight-Control-Board-DSM2-For-Inductrix-Tiny-Whoop-Eachine-E010-p-1089021.html)
- [Open Pilot Revo (F4)](https://hobbyking.com/en_us/openpilot-cc3d-revolution-revo-32bit-flight-controller-w-integrated-433mhz-oplink.html/?___store=en_us)

## Resources ##
- [ROSflight](https://github.com/byu-magicc/ROSflight)
- [BreezySTM32](https://github.com/simondlevy/BreezySTM32)
- [f3-discovery-basic-template](https://github.com/mblythe86/stm32f3-discovery-basic-template)

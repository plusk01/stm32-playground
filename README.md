STM32 Playground
================

This repo is to help me get up to speed on (and remember) various facets of programming STM32 devices. My end goal is to contribute to STM32-based flight controllers and autopilots such as [ROSflight](https://github.com/byu-magicc/ROSflight). In particular, I will be developing on the [BeeCore F3](http://www.banggood.com/Beecore-F3_EVO_Brushed-ACRO-Flight-Control-Board-DSM2-For-Inductrix-Tiny-Whoop-Eachine-E010-p-1089021.html).

## Getting Started ##

A great place to start is with the `minimalist` directory. It is an independent STM32 project that uses no peripherals and no magic. Undertanding these basics wil help to gain an intuition on what all the drivers, libraries, `Makefile`s, and linker scripts are doing.

Then, to see the use of the `CMSIS` and `StdPeriph_Driver` libs in action, take a look at the `f3` folder.

## Understanding `lib/CMSIS`, `lib/StdPeriph` and the HAL ##

There are various ways of working with ARM cores in general and the STM32 micros in particular. The ARM company creates standards for so-called ARM processors, which are then implemented by other companies (i.e., STMicroelectronics). ARM has standardized the interface to core features (e.g., registers) through a vendor-independent, header-only hardware abstraction layer called [Cortex Microontroller Software Interface Standard (CMSIS)](https://developer.arm.com/embedded/cmsis), vendors are then able to build off of that, providing further abstractions to components and peripherals that they have chosen to put into their implementation of a given ARM processor. For STM32-based micros, this extended support layer has historically been the Standard Peripheral Library (StdPeriph, or SPL); however, within the last five years STM has been pushing their new product, STM32CubeMX, with its associated HAL as a successor to the SPL.

It seems that the flight controller community has not yet adopted hardware implementations using the Cube and the new HAL, so most of the time you will find a directory named something like `StdPeriph` in the source code of projects. Further, it can be confusing knowing what to do with the `system_stm32f30x.c` implementation file. In my opinion, it should be included with the source code and NOT in the library code.

See the discussion on SO [CMSIS vs HAL vs Standard Peripherals Library](https://electronics.stackexchange.com/questions/224355/cmsis-vs-hal-vs-standard-peripherals-library).

## Obtaining Official Libraries and Examples ##

Finding the official STM32 CMSIS, StdPeriph, and examples can be a pain. However, if you search for *stm32 standard peripheral libraries*, you may be fortunate to find [this](http://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html?querycriteria=productId=LN1939) page. Once you get here, you can find the latest library and example code by clicking the **STM32 Standard Peripheral Libraries Expansion** button and filtering for your specific STM32 family.

Note that information can also be found by clicking one of the family buttons on the first page. However, the **F3** button seems to only have information for STM32F37xx/8xx. If you download an example through the **Expansion** button for the F3 (like the USB example, *STSW-STM32121*) you can find up to date CMSIS and StdPeriph libraries.

## Relevant Flight Controller Boards ##
- [Naze32 / Flip32 (F103)](http://www.readytoflyquads.com/the-flip32)
- [BeeCore F3](http://www.banggood.com/Beecore-F3_EVO_Brushed-ACRO-Flight-Control-Board-DSM2-For-Inductrix-Tiny-Whoop-Eachine-E010-p-1089021.html)
- [Open Pilot Revo (F4)](https://hobbyking.com/en_us/openpilot-cc3d-revolution-revo-32bit-flight-controller-w-integrated-433mhz-oplink.html/?___store=en_us)

## Resources ##
- [ROSflight](https://github.com/byu-magicc/ROSflight)
- [BreezySTM32](https://github.com/simondlevy/BreezySTM32)
- [f3-discovery-basic-template](https://github.com/mblythe86/stm32f3-discovery-basic-template)

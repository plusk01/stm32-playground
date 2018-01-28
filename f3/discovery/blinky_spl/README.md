Blinky Using the Standard Perhipheral Library (SPL)
===================================================

This simple example uses the code from [this](http://fos.cmb.ac.lk/esl/tutorial-4-blinking-led-using-library-functions/) tutorial.

notice the two major differences between the execution of this code and the `blinky_nospl` code:

1. The LED blinks faster
    - This comes from the `system_stm32f30x.c` file and the *reset clock control* code therein.
    - By default, the processor clock (`HCLK`) is set to system clock (`SYSCLK`), which is just the High Speed Internal (`HSI`) oscillator, which is 8MHz.
    - When using StdPeriph, the clocks are set up so that no matter what, the clock rate (`SystemCoreClock`) is 72MHz.
    - The Discovery board uses the HSE_BYPASS mechanism to allow use of the 8MHz ST-Link clock output (`MCO`) from the ST-Link's STM32F10x processor
    - This HSE_BYPASS is set at the top of thet `system_stm32f30x.c` file and should be changed for a different board.
2. The LED is more dim
    - This is because of the output mode of the GPIO pin -- open-drain with a pull-up. By default, (and in the case of `blinky_nospl`) the GPIO port is set to be push-pull with no pull-up resistor.
    - The schematic of the discovery board shows a 680 Ohm resistor in series with the LED, which is then tied to ground. By adding the extra resistor for the pull-up, we are limiting the current available to the LED.
    - Open-drain with no pull-up would never turn on because the positive side of the LED is either tied to ground or floating.
    - This could be remedied by not driving an LED directly via the output pin, but going through a buffer instead. But hey, this is an evaluation board.
    - More discussion / images of GPIO output/input types, see [this SO question](https://electronics.stackexchange.com/questions/28091/push-pull-open-drain-pull-up-pull-down).
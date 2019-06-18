#include <stm32f30x.h>

uint32_t SystemCoreClock = 0;
void SystemInit(void) {}
void SystemCoreClockUpdate(void) {}

int main(void)
{
    volatile unsigned int i = 0;

    RCC->AHBENR |=  1<<21;  // Enable CLK for port E 
    GPIOE->MODER |= 1<<26;  // Enable PORTE pin 13 as digital output 
    GPIOE->ODR |=   1<<13;  // Turn GPIOE Pin 13 ON

    while(1) {
        for (i = 0; i < 1000000; ++i); GPIOE->ODR |=  (1<<13); // Turn GPIOE Pin 13 ON
        for (i = 0; i < 1000000; ++i); GPIOE->ODR &= ~(1<<13); // Turn GPIOE Pin 13 OFF
    }
}
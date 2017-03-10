#include <stm32f30x.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_gpio.h>
 
#define LED    GPIO_Pin_8
 
int main()
{
      long i;
      GPIO_InitTypeDef gpio;
 
      // Blue LED is connected to port E, pin 8 (AHB bus)
      RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOE, ENABLE );
 
      // Configure port E (LED)
      GPIO_StructInit( &gpio );
      gpio.GPIO_Mode = GPIO_Mode_OUT;
      gpio.GPIO_Pin  = LED;
      GPIO_Init( GPIOE, &gpio );
 
      // Blinking LEDS
      while(1)
      {
              // On
              GPIO_SetBits( GPIOE, LED );
              for( i = 0; i < 500000; i++ );
              // All off
              GPIO_ResetBits( GPIOE, LED );
              for( i = 0; i < 500000; i++ );
      }
}

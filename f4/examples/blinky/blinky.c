#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
 
int main()
{
      long i;
      GPIO_InitTypeDef gpio;
 
      // Blue LED is connected to port B, pin 4 (AHB1 bus)
      RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
 
      // Configure port E (LED)
      GPIO_StructInit( &gpio );
      gpio.GPIO_Mode = GPIO_Mode_OUT;
      gpio.GPIO_Pin  = GPIO_Pin_4;
      GPIO_Init( GPIOB, &gpio );
 
      // Blinking LEDS
      while(1)
      {
              // On
              GPIO_SetBits( GPIOB, GPIO_Pin_4 );
              for( i = 0; i < 500000; i++ );
              // All off
              GPIO_ResetBits( GPIOB, GPIO_Pin_4 );
              for( i = 0; i < 500000; i++ );
      }
}

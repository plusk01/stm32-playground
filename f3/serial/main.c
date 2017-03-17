// See: http://pandafruits.com/stm32_primer/stm32_primer_uart.php
#include "stm32f30x.h"

// PB8  -- LED
// PA10 -- USART1_RX
// PA9  -- USART1_TX
 
/* User defined function prototypes */
void GPIOB_Init(void);
void USART1_Init(void);
void led_toggle(void);
 
int main(void)
{
    /* Initialize GPIOB PIN8 */
    GPIOB_Init();
    /* Initialize USART1 */
    USART1_Init();

    USART2_Init();

    long i;
 
    while(1)
    {
        /* Do nothing, all happens in ISR */
      USART_SendData(USART2, 't');
      USART_SendData(USART1, 't');
      led_toggle();
      for( i = 0; i < 500000; i++ );
    }
}   
 
/***********************************************
 * Initialize GPIOA PIN8 as push-pull output
 *
 * http://community.silabs.com/t5/8-bit-MCU/Push-Pull-vs-Open-Drain/td-p/40125
 ***********************************************/
void GPIOB_Init(void)
{
    /* Bit configuration structure for GPIOB PIN8 */
    GPIO_InitTypeDef gpiob_init_struct = { GPIO_Pin_8, GPIO_Mode_OUT, 
                                          GPIO_Speed_50MHz, GPIO_OType_PP,
                                          GPIO_PuPd_NOPULL };
                                             
    /* Enable PORT A clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* Initialize GPIOB: 50MHz, PIN8, Push-pull Output */
    GPIO_Init(GPIOB, &gpiob_init_struct);   
     
    /* Turn off LED to start with */
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}
 
/*****************************************************
 * Initialize USART1: enable interrupt on reception
 * of a character
 *****************************************************/
void USART1_Init(void)
{
    /* USART configuration structure for USART1 */
    USART_InitTypeDef usart1_init_struct;
    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef gpiob_init_struct;
     
    /* Enalbe clock for USART1, AFIO and GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
                            
    /* GPIOA PIN9 alternative function Tx */
    gpiob_init_struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gpiob_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpiob_init_struct.GPIO_Mode = GPIO_Mode_AF;
    gpiob_init_struct.GPIO_OType = GPIO_OType_PP;
    gpiob_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpiob_init_struct);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
 

    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart1_init_struct.USART_BaudRate = 9600;   
    usart1_init_struct.USART_WordLength = USART_WordLength_8b;  
    usart1_init_struct.USART_StopBits = USART_StopBits_1;   
    usart1_init_struct.USART_Parity = USART_Parity_No ;
    usart1_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART1, &usart1_init_struct);
    /* Enable RXNE interrupt */
    USART_ClearITPendingBit(s->USARTx, USART_IT_RXNE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);


    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
}

void USART2_Init(void) {
  /* USART configuration structure for USART2 */
  USART_InitTypeDef usart2_init_struct;
  /* Bit configuration structure for GPIOA PIN2 and PIN3 */
  GPIO_InitTypeDef gpiob_init_struct;
   
  /* Enalbe clock for USART1, AFIO and GPIOA */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

                          
  /* GPIOA PIN9 alternative function Tx */
  gpiob_init_struct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  gpiob_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
  gpiob_init_struct.GPIO_Mode = GPIO_Mode_AF;
  gpiob_init_struct.GPIO_OType = GPIO_OType_PP;
  gpiob_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &gpiob_init_struct);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);
  
  /* Enable USART2 */
  USART_Cmd(USART2, ENABLE);  
  /* Baud rate 9600, 8-bit data, One stop bit
   * No parity, Do both Rx and Tx, No HW flow control
   */
  usart2_init_struct.USART_BaudRate = 9600;   
  usart2_init_struct.USART_WordLength = USART_WordLength_8b;  
  usart2_init_struct.USART_StopBits = USART_StopBits_1;   
  usart2_init_struct.USART_Parity = USART_Parity_No ;
  usart2_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  usart2_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* Configure USART1 */
  USART_Init(USART2, &usart2_init_struct);
  /* Enable RXNE interrupt */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  /* Enable USART1 global interrupt */
  NVIC_EnableIRQ(USART2_IRQn);
}
 
/*******************************************
 * Toggle LED 
 *******************************************/
void led_toggle(void)
{
    /* Read LED output (GPIOB PIN8) status */
    uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8);
     
    /* If LED output set, clear it */
    if(led_bit == (uint8_t)Bit_SET)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    }
    /* If LED output clear, set it */
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
    }
}
 
/**********************************************************
 * USART1 interrupt request handler: on reception of a 
 * character 't', toggle LED and transmit a character 'T'
 *********************************************************/
void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        /* If received 't', toggle LED and transmit 'T' */
        if((char)USART_ReceiveData(USART1) == 't')
        {
            led_toggle();
            USART_SendData(USART1, 'T');
            /* Wait until Tx data register is empty, not really 
             * required for this example but put in here anyway.
             */
            /*
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            {
            }*/
        }
    }
     
    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...             */
}

/**********************************************************
 * USART1 interrupt request handler: on reception of a 
 * character 't', toggle LED and transmit a character 'T'
 *********************************************************/
void USART2_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        /* If received 't', toggle LED and transmit 'T' */
        if((char)USART_ReceiveData(USART2) == 't')
        {
            led_toggle();
            USART_SendData(USART2, 'T');
            /* Wait until Tx data register is empty, not really 
             * required for this example but put in here anyway.
             */
            /*
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            {
            }*/
        }
    }
     
    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...             */
}   
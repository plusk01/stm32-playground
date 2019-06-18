// See: http://pandafruits.com/stm32_primer/stm32_primer_uart.php
#include <stm32f4xx.h>

// PB4  -- LED
// PB11 -- USART3_RX
// PB10 -- USART3_TX
 
/* User defined function prototypes */
void GPIOB_Init(void);
void USART3_Init(void);
void led_toggle(void);
void alive_init(void);
void alive_toggle(void);
 
int main(void)
{
    /* Initialize GPIOB PIN4/5 */
    GPIOB_Init(); alive_init();
    /* Initialize USART3 */
    USART3_Init();

    long i;
 
    while(1)
    {
        /* Do nothing, all happens in ISR */
      USART_SendData(USART3, 't');
      led_toggle();
      for( i = 0; i < 500000; i++ );
    }
}   
 
/***********************************************
 * Initialize GPIOB PIN4 as push-pull output
 *
 * http://community.silabs.com/t5/8-bit-MCU/Push-Pull-vs-Open-Drain/td-p/40125
 ***********************************************/
void GPIOB_Init(void)
{
    /* Bit configuration structure for GPIOB PIN8 */
    GPIO_InitTypeDef gpiob_init_struct = { GPIO_Pin_4, GPIO_Mode_OUT, 
                                          GPIO_Speed_50MHz, GPIO_OType_PP,
                                          GPIO_PuPd_NOPULL };
                                             
    /* Enable PORT B clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Initialize GPIOB: 50MHz, PIN8, Push-pull Output */
    GPIO_Init(GPIOB, &gpiob_init_struct);   
     
    /* Turn off LED to start with */
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}

/***********************************************
 * Initialize GPIOB PIN5 as push-pull output
 ***********************************************/
void alive_init(void)
{
    /* Bit configuration structure for GPIOB PIN8 */
    GPIO_InitTypeDef gpiob_init_struct = { GPIO_Pin_5, GPIO_Mode_OUT, 
                                          GPIO_Speed_50MHz, GPIO_OType_PP,
                                          GPIO_PuPd_NOPULL };
                                             
    /* Enable PORT B clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Initialize GPIOB: 50MHz, PIN8, Push-pull Output */
    GPIO_Init(GPIOB, &gpiob_init_struct);   
     
    /* Turn off LED to start with */
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}
 
/*****************************************************
 * Initialize USART3: enable interrupt on reception
 * of a character
 *****************************************************/
void USART3_Init(void)
{
    /* USART configuration structure for USART3 */
    USART_InitTypeDef usart3_init_struct;
    /* Bit configuration structure for GPIOB PIN9 and PIN10 */
    GPIO_InitTypeDef gpiob_init_struct;
     
    /* Enalbe clock for USART3, AFIO and GPIOB */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
                            
    /* GPIOB PIN10 alternative function Tx */
    gpiob_init_struct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
    gpiob_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpiob_init_struct.GPIO_Mode = GPIO_Mode_AF;
    gpiob_init_struct.GPIO_OType = GPIO_OType_PP;
    gpiob_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gpiob_init_struct);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
 

    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart3_init_struct.USART_BaudRate = 115200;   
    usart3_init_struct.USART_WordLength = USART_WordLength_8b;  
    usart3_init_struct.USART_StopBits = USART_StopBits_1;   
    usart3_init_struct.USART_Parity = USART_Parity_No ;
    usart3_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart3_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART3 */
    USART_Init(USART3, &usart3_init_struct);
    /* Enable RXNE interrupt */
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    /* Enable USART3 global interrupt */
    NVIC_EnableIRQ(USART3_IRQn);


    /* Enable USART3 */
    USART_Cmd(USART3, ENABLE);
}
 
/*******************************************
 * Toggle LED 
 *******************************************/
void led_toggle(void)
{
    /* Read LED output (GPIOB PIN4) status */
    uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4);
     
    /* If LED output set, clear it */
    if(led_bit == (uint8_t)Bit_SET)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    }
    /* If LED output clear, set it */
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_4);
    }
}

/*******************************************
 * Toggle ALIVE LED 
 *******************************************/
void alive_toggle(void)
{
    /* Read LED output (GPIOB PIN5) status */
    uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5);
     
    /* If LED output set, clear it */
    if(led_bit == (uint8_t)Bit_SET)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    }
    /* If LED output clear, set it */
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
    }
}
 
/**********************************************************
 * USART3 interrupt request handler: on reception of a 
 * character 't', toggle LED and transmit a character 'T'
 *********************************************************/
void USART3_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        /* If received 't', toggle LED and transmit 'T' */
        if((char)USART_ReceiveData(USART3) == 't')
        {
            alive_toggle();
            USART_SendData(USART3, 'T');
            /* Wait until Tx data register is empty, not really 
             * required for this example but put in here anyway.
             */
            /*
            while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
            {
            }*/
        }
    }
     
    /* ------------------------------------------------------------ */
    /* Other USART3 interrupts handler can go here ...             */
}
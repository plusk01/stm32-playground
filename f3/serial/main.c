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
 
    while(1)
    {
        /* Do nothing, all happens in ISR */
    }
}   
 
/***********************************************
 * Initialize GPIOA PIN8 as push-pull output
 *
 * http://community.silabs.com/t5/8-bit-MCU/Push-Pull-vs-Open-Drain/td-p/40125
 ***********************************************/
void GPIOB_Init(void)
{
    /* Bit configuration structure for GPIOA PIN8 */
    GPIO_InitTypeDef gpiob_init_struct = { GPIO_Pin_8, GPIO_Speed_50MHz, 
                                           GPIO_Mode_Out_PP };
                                             
    /* Enable PORT A clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* Initialize GPIOB: 50MHz, PIN8, Push-pull Output */
    GPIO_Init(GPIOA, &gpiob_init_struct);   
     
    /* Turn off LED to start with */
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | 
                           RCC_APB2Periph_GPIOA, ENABLE);
                            
    /* GPIOA PIN9 alternative function Tx */
    gpiob_init_struct.GPIO_Pin = GPIO_Pin_9;
    gpiob_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpiob_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpiob_init_struct);
    /* GPIOA PIN9 alternative function Rx */
    gpiob_init_struct.GPIO_Pin = GPIO_Pin_10;
    gpiob_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpiob_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpiob_init_struct);
 
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);  
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
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);
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
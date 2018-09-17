#include "usart6.h"

/*-----USART6_TX-----PC6-----*/
/*-----USART6_RX-----PC7-----*/

static FIFO_S_t* UART_TranFifo;

void USART6_Configuration(void)
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    gpio.GPIO_Pin                           =   GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Mode                          =   GPIO_Mode_AF;
    gpio.GPIO_OType                         =   GPIO_OType_PP;
    gpio.GPIO_Speed                         =   GPIO_Speed_100MHz;
    gpio.GPIO_PuPd                          =   GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &gpio);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); 
    
    usart.USART_BaudRate                    =   115200;
    usart.USART_WordLength                  =   USART_WordLength_8b;
    usart.USART_StopBits                    =   USART_StopBits_1;
    usart.USART_Parity                      =   USART_Parity_No;
    usart.USART_Mode                        =   USART_Mode_Tx | USART_Mode_Rx;
    usart.USART_HardwareFlowControl         =   USART_HardwareFlowControl_None;
    USART_Init(USART6, &usart);


    nvic.NVIC_IRQChannel                    =   USART6_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   1;
    nvic.NVIC_IRQChannelSubPriority         =   1;
    nvic.NVIC_IRQChannelCmd                 =   ENABLE; 
    NVIC_Init(&nvic);
    
    USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART6,ENABLE);
    
    UART_TranFifo = FIFO_S_Create(100);
}

void USART6_IRQHandler(void)
{  
    if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
    {   
        if(!FIFO_S_IsEmpty(UART_TranFifo))
        {
			uint16_t data = (uint16_t)FIFO_S_Get(UART_TranFifo);
			USART_SendData(USART6, data);
        }
        else
        {
			USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
        }  
    }
	else if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)   
    {
		uint8_t data_get = USART_ReceiveData(USART6);
        imu_download_msg_process(data_get);
    }     			
}

void UART6_PrintCh(uint8_t ch)
{    
    FIFO_S_Put(UART_TranFifo, ch);
    USART_ITConfig(USART6,USART_IT_TXE, ENABLE);
}

void UART6_PutStr (const char *str)
{
    while(*str)
    {
		USART_SendData(USART6, *str++); 
		while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET){}
    }
}

void UART6_PrintBlock(uint8_t* pdata, uint8_t len)
{
	uint8_t i = 0;
    for(i = 0; i < len; i++)
    {
        FIFO_S_Put(UART_TranFifo, pdata[i]);
    }
    USART_ITConfig(USART6, USART_IT_TXE, ENABLE);  
}


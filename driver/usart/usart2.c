#include "usart2.h"

/*-----USART2_TX-----PD5-----*/
/*-----USART2_RX-----PD6-----*/

/**
 *  裁判系统信息获取渠道
 *  
 */

static FIFO_S_t* UART_TranFifo;

void USART2_Configuration(void)
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,  ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    gpio.GPIO_Pin                           =   GPIO_Pin_5 | GPIO_Pin_6;
    gpio.GPIO_Mode                          =   GPIO_Mode_AF;
    gpio.GPIO_OType                         =   GPIO_OType_PP;
    gpio.GPIO_Speed                         =   GPIO_Speed_100MHz;
    gpio.GPIO_PuPd                          =   GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &gpio);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 

    usart.USART_BaudRate                    =   115200;
    usart.USART_WordLength                  =   USART_WordLength_8b;
    usart.USART_StopBits                    =   USART_StopBits_1;
    usart.USART_Parity                      =   USART_Parity_No;
    usart.USART_Mode                        =   USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl         =   USART_HardwareFlowControl_None;
    USART_Init(USART2, &usart);

    nvic.NVIC_IRQChannel                    =   USART2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  =   0;
    nvic.NVIC_IRQChannelSubPriority         =   0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    
    USART_Cmd(USART2, ENABLE);

    UART_TranFifo = FIFO_S_Create(100);  //申请内存
}

void USART2_IRQHandler(void)
{  
    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {
        // 排队 
        if(!FIFO_S_IsEmpty(UART_TranFifo))
        {
            uint16_t data = (uint16_t)FIFO_S_Get(UART_TranFifo);
            USART_SendData(USART2, data);
        }
        // 无数据
        else
        {
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }
    }
    else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //接收中断 接收池非空
    {
        uint8_t ucTemp = USART_ReceiveData(USART2);
        js_download_msg_process(ucTemp);
    }
}

void USART2_PrintBlock(uint8_t* pdata, uint8_t len)
{
    uint8_t i = 0;
    for(i = 0; i < len; i++)
    {
        FIFO_S_Put(UART_TranFifo, pdata[i]);
    }
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  //发送寄存器空中断
}


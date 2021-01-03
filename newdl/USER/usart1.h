#ifndef __USART1_H
#define	__USART1_H



#include "stm32f10x.h"
#include <stdio.h>
#define USART1_DR_Base  0x40013804
#define USART2_DR_Base  0x40004404
#define SENDBUFF_SIZE0   20

#define USART2_RE_high GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define USART2_RE_lowx GPIO_ResetBits(GPIOA, GPIO_Pin_5)

#define USART2_DE_high GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define USART2_DE_lowx GPIO_ResetBits(GPIOA, GPIO_Pin_1)

#define USART3_RE_high GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define USART3_RE_lowx GPIO_ResetBits(GPIOC, GPIO_Pin_5)

#define USART3_DE_high GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define USART3_DE_lowx GPIO_ResetBits(GPIOA, GPIO_Pin_7)



//#define USART2_CT_high GPIO_SetBits(GPIOA, GPIO_Pin_7)
//#define USART2_CT_lowx GPIO_ResetBits(GPIOA, GPIO_Pin_7)
//
//#define USART2_RT_high GPIO_SetBits(GPIOC, GPIO_Pin_3)
//#define USART2_RT_lowx GPIO_ResetBits(GPIOC, GPIO_Pin_3)


 
void USART1_Config(void);
void USART2_Config(void);
void USART3_Config(void);
void UART4_Config(void);
void UART5_Config(void);
void USART1_REMAP_Config(void);


int fputc(int ch, FILE *f);

void USART1_Process(void);
void USART2_Process(void);
void USART3_Process(void);
void UART4_Process(void);
void UART5_Process(void);


void initi_usart2_control_pin(void);


void NVIC_Configuration(void);
void RCC_Configuration(void);
void DMA_Configuration(char SendBuff[],u16 SENDBUFF_SIZE);
void DMA_Configuration_for_recive(char SendBuff[],u16 SENDBUFF_SIZE);
void Key_GPIO_Config(void);

#endif /* __USART1_H */

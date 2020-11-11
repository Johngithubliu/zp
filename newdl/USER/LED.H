#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

#define LED1_OFF GPIO_SetBits(GPIOA,GPIO_Pin_8);    //PA8输出高电平
#define LED1_ON GPIO_ResetBits(GPIOA,GPIO_Pin_8);   //PA8输出低电平
#define LED2_OFF GPIO_SetBits(GPIOD,GPIO_Pin_2);    //PD2输出高电平
#define LED2_ON GPIO_ResetBits(GPIOD,GPIO_Pin_2);   //PD2输出低电平
#define LED3_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12);   //PC12输出高电平
#define LED3_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12);  //PC12输出低电平

void LED_GPIO_Config(void);   /* LED 端口初始化 */

#endif /* __LED_H */

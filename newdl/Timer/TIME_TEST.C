/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/

#include "timer.h"
#include "led.h"

/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
void TIME_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/*设置NVIC中断分组2:2位抢占优先级，2位响应优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	
	/*TIM3中断*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
  /*先占优先级0级*/	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;// 0; 
  /*从优先级3级*/	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//1;//3; 
  /*IRQ通道被使能*/	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	/*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器*/
	NVIC_Init(&NVIC_InitStructure);  
}


void TIME_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/*定时器TIM3时钟使能*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
  /*设置在下一个更新事件装入活动的自动重装载寄存器周期的值,计数到5000为500ms*/
	TIM_TimeBaseStructure.TIM_Period =50;// 5000; 
	/*设置用来作为TIMx时钟频率除数的预分频值*/
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1);
  /*设置时钟分割:TDTS = Tck_tim*/  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	/*TIM向上计数模式*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  /*根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位*/
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
  /*使能或者失能指定的TIM中断*/
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	/*使能TIMx外设*/
	TIM_Cmd(TIM3, ENABLE);  
}

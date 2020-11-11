/******************** ��ѧ�Ƽ� **************************
 * ʵ��ƽ̨��������STM32������
 * ��汾  ��ST3.5.0
 * ����    ����ѧ�Ƽ��Ŷ� 
 * �Ա�    ��http://shop102218275.taobao.com/
 * ������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
 *��Ȩ���У�����ؾ���
**********************************************************************************/

#include "timer.h"
#include "led.h"

/*
 * ��������TIM2_NVIC_Configuration
 * ����  ��TIM2�ж����ȼ�����
 * ����  ����
 * ���  ����	
 */
void TIME_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/*����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	
	/*TIM3�ж�*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
  /*��ռ���ȼ�0��*/	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;// 0; 
  /*�����ȼ�3��*/	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//1;//3; 
  /*IRQͨ����ʹ��*/	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	/*����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���*/
	NVIC_Init(&NVIC_InitStructure);  
}


void TIME_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/*��ʱ��TIM3ʱ��ʹ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
  /*��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,������5000Ϊ500ms*/
	TIM_TimeBaseStructure.TIM_Period =50;// 5000; 
	/*����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ*/
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1);
  /*����ʱ�ӷָ�:TDTS = Tck_tim*/  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	/*TIM���ϼ���ģʽ*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  /*����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ*/
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
  /*ʹ�ܻ���ʧ��ָ����TIM�ж�*/
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	/*ʹ��TIMx����*/
	TIM_Cmd(TIM3, ENABLE);  
}
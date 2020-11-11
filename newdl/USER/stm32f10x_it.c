/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h"
#include "stdbool.h"
#include "usart1.h"
extern delay_tare_timer;
extern void Weight_checking(int k);
extern u8 W_Totall;
extern int frezen_AD_DISPLAY_DELAY_TIMER;
int Modbus_monitor;
extern void Input_Filter(void);
extern void USART1_Process(void);
extern void USART2_Process(void);
extern void USART3_Process(void);
extern void UART4_Process(void);
extern void UART5_Process(void);


extern void	sent_x_state_to_y_state(void);
extern void	Y_relay_state(void);
extern void			  simulator_package(void);//输出
extern void			  Store_XRCT_Status(void);//边沿采集
#define Timer_num 50
extern void Timer_run(u8 k);
extern bool T_timer_trigger[Timer_num];

#define Command_01 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define Command_02 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define Un_N_package_output     Y_output_state[8]=0

extern void Step_checking(int k);
extern u8 S_Totall;
extern u32 ReadADresult(void);
extern void WRITEBYTE(unsigned char byte);
unsigned long AD_Result0;
unsigned long AD_QUEEN[40];
char QUEEN;

extern void Modbus_input(void);	
extern void Modbus_output(void);
extern char Eliminate_zero_delay;

extern int Range_Filter_reg,Strong_Filter_reg;
extern char waitting_for_KLTT_PARA;

extern int err_setting_Slow_feed_timer;
/****************************************************/
extern void Scanning_input_KLTT(void);
extern void Teating_weight_Random_output(void);
extern void Teating_weight_zero(void);
extern void process_feed_material(void);
extern void Test_input_edge(void);
extern void KLTT_Test_input_edge(void);
/****************************************************/	
extern void AD_F_process(void);
char sys_timerx;
extern char c_t100,auto_t100,t100;
extern char  Y_output_state[50];	   //bool
extern char elimate_USART_DE[6];
#define USART2_DE_lowx GPIO_ResetBits(GPIOA, GPIO_Pin_1)
extern  unsigned char supervision_ad_com;
char executive_contorl;

extern void do_flashing(unsigned char flash_ponter);
extern char flash_ponter,flash_ponter_image;
extern void Setting_configur(void);
void Key_input_edge(void);
void signal_convert_to_LED (void);
extern void display_main_weight(void);
extern int shut_initial_mid_slow_prefeed_timer[4];  //*2017-10-28am*
extern int stop_dis_AD_timer;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
void USART1_IRQHandler(void)
{
	USART1_Process();
}
void USART2_IRQHandler(void)
{
	USART2_Process();
}

void USART3_IRQHandler(void)
{
	USART3_Process();
}
void UART4_IRQHandler(void)
{
	UART4_Process();
}
void UART5_IRQHandler(void)
{
	UART5_Process();
}
 /******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/*TIM3中断服务函省*/
void TIM3_IRQHandler(void)   
{
unsigned char k;
	 /*检查指定的TIM中断发生与否*/
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
		{
			 /*清除TIMx的中断待处理位*/
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 
//				 if(Command_01)return;
//				display_main_weight();
		

			/*控制LED的IO翻转*/
		Input_Filter();
	    if(Modbus_monitor)Modbus_monitor--;	 //5ms
    	else {GPIO_WriteBit(GPIOE, GPIO_Pin_5, (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5))));Modbus_monitor=6;
//				do_flashing(flash_ponter_image);
//				Setting_configur();
//			    Key_input_edge();
//				signal_convert_to_LED ();
//				display_main_weight();
			}
		if(supervision_ad_com<20)
		  {supervision_ad_com++;}
		else
			{supervision_ad_com=0;
//			Save_para_Initial_zero_range();
//			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//			USART_ClearFlag(USART2, USART_IT_RXNE);
			__set_FAULTMASK(1);   // 关闭所有中端
			NVIC_SystemReset();   // 复位
			}

		 if(sys_timerx<20)
		     {sys_timerx++;}
		 else
		     {sys_timerx=0;
			  if (c_t100)c_t100--;
			  if(stop_dis_AD_timer)stop_dis_AD_timer--;
			  if(err_setting_Slow_feed_timer)err_setting_Slow_feed_timer--;
			  if(delay_tare_timer)delay_tare_timer--;
			  if(frezen_AD_DISPLAY_DELAY_TIMER)frezen_AD_DISPLAY_DELAY_TIMER--;


			  if(shut_initial_mid_slow_prefeed_timer[0])shut_initial_mid_slow_prefeed_timer[0]--;  //*2017-10-28am* 
			  if(shut_initial_mid_slow_prefeed_timer[1])shut_initial_mid_slow_prefeed_timer[1]--;  //*2017-10-28am* 
			  if(shut_initial_mid_slow_prefeed_timer[2])shut_initial_mid_slow_prefeed_timer[2]--;  //*2017-10-28am* 
			  if(shut_initial_mid_slow_prefeed_timer[3])shut_initial_mid_slow_prefeed_timer[3]--;  //*2017-10-28am* 
			  if (auto_t100)auto_t100++;
			  if (t100)t100--;
			  else 	 Un_N_package_output;
			  	 
			 } 

		  if(elimate_USART_DE[2]>1)
		   { elimate_USART_DE[2]--; 
		    if(elimate_USART_DE[2]==1)
			{USART2_DE_lowx;elimate_USART_DE[2]--;}	
		   }
		  if(elimate_USART_DE[3]>1)
		   { elimate_USART_DE[3]--; 
		    if(elimate_USART_DE[3]==1)
			{USART2_DE_lowx;elimate_USART_DE[3]--;}	
		   }
		  if(elimate_USART_DE[4]>1)
		   { elimate_USART_DE[4]--; 
		    if(elimate_USART_DE[4]==1)
			{USART2_DE_lowx;elimate_USART_DE[4]--;}	
		   }		   
		  if(elimate_USART_DE[5]>1)
		   { elimate_USART_DE[5]--; 
		    if(elimate_USART_DE[5]==1)
			{USART2_DE_lowx;elimate_USART_DE[5]--;}	
		   }		   
		   
		   	//		if(Modbus_monitor==1)//15个定时器.1s
//			{
//			  for(k=0;k<W_Totall;k++)
//			   {
//			    Weight_checking(k);//扫描重量元件
//			   }
//
//			  for(k=0;k<S_Totall;k++)
//			   {
//			    Step_checking(k);//扫描步进器
//			   }
//			  for(k=0;k<15;k++)
//			   {
//				if(T_timer_trigger[k])
//				   Timer_run(k);//扫描定时器
//			   }
//				  Input_Filter();
//
//				  Modbus_input(); //for modbus input demstoed
//				  Modbus_output();//for modbus output demstoed
//			}

			if(Modbus_monitor==2)//60ms sampling input   
				{
				/****************************************************/
//			       while(!waitting_for_KLTT_PARA)return;

					Scanning_input_KLTT();
					Teating_weight_Random_output();
					Teating_weight_zero();
					process_feed_material();
//					executive_contorl=1;
					Test_input_edge();
					KLTT_Test_input_edge();
//					Y_relay_state();
//					Modbus_input(); //for modbus input demstoed
//					Modbus_output();//for modbus output demstoed
				/****************************************************/
				if(Eliminate_zero_delay)
				  { Eliminate_zero_delay--;}

				}
//			if(Modbus_monitor==10)//60ms sampling input 
//			if(Modbus_monitor==5)//60ms sampling input     
//				{
//				  if(Command_02)
//				    {
//					  simulator_package();//输出
//					  Store_XRCT_Status();//边沿采集
//					}
//
//	 			  if(!Command_02)
//				    {
//					 sent_x_state_to_y_state();
//					}
////					 sent_x_state_to_y_state();
//					 Y_relay_state();
//
//			  }
			if(Modbus_monitor==5)//75ms senting output 
				{
					Y_relay_state();
					Modbus_input(); //for modbus input demstoed
					Modbus_output();//for modbus output demstoed

				}

		}
}

void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
	EXTI_ClearFlag(EXTI_Line0) ;
	EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

 void EXTI1_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
	EXTI_ClearFlag(EXTI_Line1) ;
	EXTI_ClearITPendingBit(EXTI_Line1);
  }
}
void EXTI15_10_IRQHandler(void)
{
if (EXTI_GetITStatus(EXTI_Line14) != RESET)
{
	ReadADresult();

		EXTI_ClearFlag(EXTI_Line14) ;
		EXTI_ClearITPendingBit(EXTI_Line14);
}
}

void EXTI9_5_IRQHandler(void)
{
char i;
unsigned long sum_ad;

if (EXTI_GetITStatus(EXTI_Line5) != RESET)
{
	EXTI_ClearFlag(EXTI_Line5) ;
	EXTI_ClearITPendingBit(EXTI_Line5);
}
if (EXTI_GetITStatus(EXTI_Line6) != RESET)
{
	EXTI_ClearFlag(EXTI_Line6) ;
	EXTI_ClearITPendingBit(EXTI_Line6);
}
if (EXTI_GetITStatus(EXTI_Line7) != RESET)
{
	EXTI_ClearFlag(EXTI_Line7) ;
	EXTI_ClearITPendingBit(EXTI_Line7);
}
if (EXTI_GetITStatus(EXTI_Line9) != RESET)
{
	EXTI_ClearFlag(EXTI_Line9) ;
	EXTI_ClearITPendingBit(EXTI_Line9);
}
if (EXTI_GetITStatus(EXTI_Line8) != RESET)
{
	AD_Result0=ReadADresult();
	supervision_ad_com=0;
		   GPIO_WriteBit(GPIOE, GPIO_Pin_6, (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_6))));//第一LED:通讯
 	       if(Strong_Filter_reg&&Range_Filter_reg)
		     {
	            AD_F_process();
			 }
	EXTI_ClearFlag(EXTI_Line8) ;
	EXTI_ClearITPendingBit(EXTI_Line8);

}
}

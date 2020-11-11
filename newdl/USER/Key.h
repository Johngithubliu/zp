
/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/


#ifndef __KEY_H
#define __KEY_H	 


#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)   //读PA13
#define KEY2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//读PA0  WK_UP
/*PWC input output*/
#define X_input_00 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define X_input_01 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)
#define X_input_02 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)
#define X_input_03 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)
#define X_input_04 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)
#define X_input_05 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)
#define X_input_06 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)
#define X_input_07 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)

#define X_input_08 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define X_input_09 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11)
#define X_input_0A GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_13)
#define X_input_0B GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_15)
#define X_input_0C GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1)
#define X_input_0D GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)
#define X_input_0E GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)
#define X_input_0F GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)

/*拨盘输入**/
#define Command_01 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define Command_02 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define Command_03 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define Command_04 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)


														
#define Y_output_high_00 GPIO_SetBits(GPIOE, GPIO_Pin_3)
#define Y_output_lowx_00 GPIO_ResetBits(GPIOE, GPIO_Pin_3)

#define Y_output_high_01 GPIO_SetBits(GPIOE, GPIO_Pin_2)
#define Y_output_lowx_01 GPIO_ResetBits(GPIOE, GPIO_Pin_2)

#define Y_output_high_02 GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define Y_output_lowx_02 GPIO_ResetBits(GPIOC, GPIO_Pin_13)

#define Y_output_high_03 GPIO_SetBits(GPIOF, GPIO_Pin_1)
#define Y_output_lowx_03 GPIO_ResetBits(GPIOF, GPIO_Pin_1)

#define Y_output_high_04 GPIO_SetBits(GPIOF, GPIO_Pin_3)
#define Y_output_lowx_04 GPIO_ResetBits(GPIOF, GPIO_Pin_3)

#define Y_output_high_05 GPIO_SetBits(GPIOF, GPIO_Pin_5)
#define Y_output_lowx_05 GPIO_ResetBits(GPIOF, GPIO_Pin_5)

#define Y_output_high_06 GPIO_SetBits(GPIOF, GPIO_Pin_7)
#define Y_output_lowx_06 GPIO_ResetBits(GPIOF, GPIO_Pin_7)

#define Y_output_high_07 GPIO_SetBits(GPIOF, GPIO_Pin_9)
#define Y_output_lowx_07 GPIO_ResetBits(GPIOF, GPIO_Pin_9)

#define Y_output_high_08 GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define Y_output_lowx_08 GPIO_ResetBits(GPIOB, GPIO_Pin_0)

#define Y_output_high_09 GPIO_SetBits(GPIOF, GPIO_Pin_12)
#define Y_output_lowx_09 GPIO_ResetBits(GPIOF, GPIO_Pin_12)

#define Y_output_high_0A GPIO_SetBits(GPIOF, GPIO_Pin_14)
#define Y_output_lowx_0A GPIO_ResetBits(GPIOF, GPIO_Pin_14)

#define Y_output_high_0B GPIO_SetBits(GPIOG, GPIO_Pin_0)
#define Y_output_lowx_0B GPIO_ResetBits(GPIOG, GPIO_Pin_0)

#define Y_output_high_0C GPIO_SetBits(GPIOE, GPIO_Pin_7)
#define Y_output_lowx_0C GPIO_ResetBits(GPIOE, GPIO_Pin_7)

#define Y_output_high_0D GPIO_SetBits(GPIOE, GPIO_Pin_9)
#define Y_output_lowx_0D GPIO_ResetBits(GPIOE, GPIO_Pin_9)

#define Y_output_high_0E GPIO_SetBits(GPIOE, GPIO_Pin_11)
#define Y_output_lowx_0E GPIO_ResetBits(GPIOE, GPIO_Pin_11)

#define Y_output_high_0F GPIO_SetBits(GPIOE, GPIO_Pin_13)
#define Y_output_lowx_0F GPIO_ResetBits(GPIOE, GPIO_Pin_13)

/*显示板*/
#if defined	   _2803A
#define DO_low GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define DO_high GPIO_ResetBits(GPIOC, GPIO_Pin_3)

#define CLK_low GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define CLK_high GPIO_ResetBits(GPIOA, GPIO_Pin_5)

#define STB_low0 GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define STB_high0 GPIO_ResetBits(GPIOA, GPIO_Pin_7)

#define STB_low1 GPIO_SetBits(GPIOE, GPIO_Pin_14)
#define STB_high1 GPIO_ResetBits(GPIOE, GPIO_Pin_14)

#define DI_IN GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)

#else

#define DO_high GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define DO_low GPIO_ResetBits(GPIOB, GPIO_Pin_8)

#define CLK_high GPIO_SetBits(GPIOE, GPIO_Pin_0)
#define CLK_low GPIO_ResetBits(GPIOE, GPIO_Pin_0)

#define STB_high0 GPIO_SetBits(GPIOE, GPIO_Pin_1)
#define STB_low0 GPIO_ResetBits(GPIOE, GPIO_Pin_1)

//#define STB_high1 GPIO_SetBits(GPIOB, GPIO_Pin_9)
//#define STB_low1 GPIO_ResetBits(GPIOB, GPIO_Pin_9)

#define DI_IN GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

#endif


void Out_Input_GPIO_Config(void);
void Key_GPIO_Config(void);//IO初始化
unsigned char KEY_Scan(void);  //按键扫描函数
unsigned char KEY0_Scan(void);  //按键扫描函数

unsigned char KEY2_Scan(void);  //按键扫描函数

#endif

/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/
#include "usart1.h"
#include <stdarg.h>
#include <CRC.h>
#include <modbus_para.h>
char err_setting_Slow_feed,err_setting_Pre_feed,err_setting_Target;
extern void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
extern void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
extern unsigned char *status_and_datum(void);
extern unsigned char Signal_lamp[9];
 //1.通讯数据组
#define  FF_ID_budrate  0X08072000//参数波特率关联号
#define  FF_Comm_Addr   FF_ID_budrate+4//参数通讯地址关联号
//2.置零数据组
#define  FF_Initial_zero_range  FF_Comm_Addr+4//参数开机置零范围
#define  FF_Munal_zero_range    FF_Initial_zero_range+4//参数手动置零范围
#define  FF_Trace_zero_range    FF_Munal_zero_range+4//参数零跟踪范围
//3.延时数据组
#define  FF_Delay_tare                        FF_Trace_zero_range+4//参数去皮延时输出
#define  FF_Delay_Mid_feed                    FF_Delay_tare+4//参数中加延时
#define  FF_Delay_Pre_process                 FF_Delay_Mid_feed+4//参数提前延时
#define  FF_Delay_Next_loop                   FF_Delay_Pre_process+4//参数下一循环延时
#define  FF_Time_supvisor_Slow_feed_up_limt   FF_Delay_Next_loop+4//参数慢加监视上限时间
#define  FF_Time_supvisor_Slow_feed_low_limt  FF_Time_supvisor_Slow_feed_up_limt+4//参数慢加监视下限时间
#define  FF_Pulse_howmany_package             FF_Time_supvisor_Slow_feed_low_limt+4//参数多少袋输出脉冲
#define  FF_Tare_howmany_package              FF_Pulse_howmany_package+4//参数多少袋去皮一次
#define  FF_Recive_x_signals                  FF_Tare_howmany_package+4//参数收到x次信号设定
#define  FF_no_recive_x_signals               FF_Recive_x_signals+4//参数收不到x次信号设定限
#define  FF_Even_howmany_package_Pre_process  FF_no_recive_x_signals+4//参数多少袋平均修正一次提前量
#define  FF_Delay_slow_feed                   FF_Even_howmany_package_Pre_process+4//参数慢加延时
#define  FF_Delay_initial_feed                FF_Delay_slow_feed+4//参数初加延时

//4.控制数据组  FF_ 
#define  FF_Slow_feed_modify_uper_limt  FF_Delay_initial_feed+4//参数慢加修正上限
#define  FF_Slow_feed_modify_down_limt  FF_Slow_feed_modify_uper_limt+4//参数慢加修正下限
#define  FF_Pre_feed_modify_uper_limt   FF_Slow_feed_modify_down_limt+4//参数提前量修正上限
#define  FF_Pre_feed_modify_down_limt   FF_Pre_feed_modify_uper_limt+4//参数提前量修正下限
#define  FF_Slow_feed                   FF_Pre_feed_modify_down_limt+4//参数慢加量
#define  FF_Pre_feed                    FF_Slow_feed+4//参数提前量
#define  FF_Mid_feed                    FF_Pre_feed+4//参数中加值
#define  FF_Dlet_error                  FF_Mid_feed+4//参数允差
#define  FF_Target                      FF_Dlet_error+4//参数定值
#define  FF_Target_uper_limt            FF_Target+4//参数定量上限
#define  FF_Target_down_limt            FF_Target_uper_limt+4//参数定量下限
#define  FF_zero_range                  FF_Target_down_limt+4//参数零区
#define  FF_Slow_feed_modify            FF_zero_range+4//参数慢加修正量
#define  FF_Initial_power_value         FF_Slow_feed_modify+4//参数初上电值
#define  FF_Initial_feed_value          FF_Initial_power_value+4//参数初加量
#define  FF_Random_setting_value        FF_Initial_feed_value+4//参数随机设定值

//5.修正数据组
#define  FF_ID_user_cal          FF_Random_setting_value+4//参数用户标定关联号
#define  FF_ID_slow_feed_modify  FF_ID_user_cal+4//参数慢加修正标记关联号
#define  FF_ID_pre_feed_modify   FF_ID_slow_feed_modify+4//参数提前修正标记关联号
#define  FF_ID_run_mode          FF_ID_pre_feed_modify+4//参数运行模式关联号



//6.杂项数据组
#define  FF_Clear_merory_code  FF_ID_run_mode+4//内存清理密码
#define  FF_Target_pulse       FF_Clear_merory_code+4//参数定量的总脉冲数
#define  FF_ID_weight_unit     FF_Target_pulse+4//参数重量单位设定关联号
#define  FF_Num_shift          FF_ID_weight_unit+4//参数班次
//7.#define  FF_ +4//标定数据组
#define  FF_Cal_code         FF_Num_shift+4//标定密码
#define  FF_Cal_point        FF_Cal_code+4//标定小数点
#define  FF_Cal_ID_division  FF_Cal_point+4//标定分度数关联号
#define  FF_Cal_full_Mig     FF_Cal_ID_division+4//标定满量程值
#define  FF_Cal_user_weight  FF_Cal_full_Mig+4//标定用户砝码重量
//#define  FF_Cal_zero  FF_Cal_user_weight+4//标定零位
//#define  FF_Cal_ratio  FF_Cal_zero+4//标定标率
#define  FF_Cal_weight       FF_Cal_user_weight+4	//标定砝码重量

//8.滤波数据组：
#define  FF_Range_Filter0   FF_Cal_weight+4//参数滤波范围0
#define  FF_Range_Filter1   FF_Range_Filter0+4//参数滤波范围1
#define  FF_Range_Filter2   FF_Range_Filter1+4//参数滤波范围2
#define  FF_Range_Filter3   FF_Range_Filter2+4//参数滤波范围3
#define  FF_Range_Filter4   FF_Range_Filter3+4//参数滤波范围4
#define  FF_Strong_Filter0  FF_Range_Filter4+4//参数滤波强度0
#define  FF_Strong_Filter1  FF_Strong_Filter0+4//参数滤波强度1
#define  FF_Strong_Filter2  FF_Strong_Filter1+4//参数滤波强度2
#define  FF_Strong_Filter3  FF_Strong_Filter2+4//参数滤波强度3
#define  FF_Strong_Filter4  FF_Strong_Filter3+4//参数滤波强度4
//映象参数
#define  FF_image_Pulse_howmany_package	              FF_Strong_Filter4+4//参数多少袋输出脉冲
#define  FF_image_Tare_howmany_package	              FF_image_Pulse_howmany_package+4//参数多少袋去皮一次
#define  FF_image_Even_howmany_package_Pre_process	  FF_image_Tare_howmany_package+4//参数多少袋平均修正一次提前量
#define  FF_image_Pre_feed	                          FF_image_Even_howmany_package_Pre_process+4//参数提前量

#define  FF_Analog_mode_SEL	                          FF_image_Pre_feed+4//模拟量选择




#define Test_Y00_pressed 2000-1
#define Test_Y01_pressed Test_Y00_pressed+1
#define Test_Y02_pressed Test_Y01_pressed+1
#define Test_Y03_pressed Test_Y02_pressed+1
#define Test_Y04_pressed Test_Y03_pressed+1
#define Test_Y05_pressed Test_Y04_pressed+1
#define Test_Y06_pressed Test_Y05_pressed+1
#define Test_Y07_pressed Test_Y06_pressed+1
#define Test_Y08_pressed Test_Y07_pressed+1
#define Test_Y09_pressed Test_Y08_pressed+1
#define Test_Y10_pressed Test_Y09_pressed+1
#define Test_Y11_pressed Test_Y10_pressed+1
#define Test_Y12_pressed Test_Y11_pressed+1
#define Test_Y13_pressed Test_Y12_pressed+1
#define Test_Y14_pressed Test_Y13_pressed+1
#define Test_Y15_pressed Test_Y14_pressed+1
#define Comm_opened    Test_Y15_pressed+1//打开通讯界面 
#define Zero_opened    Comm_opened +1//打开置零界面 
#define Delay_opened   Zero_opened +1//打开延时界面 
#define Contral_opened Delay_opened+1//打开控制界面 
#define Modify_opened  Contral_opened+1//打开修正界面 
#define Cal_opened     Modify_opened +1//打开标定界面 
#define Micil_opened   Cal_opened +1//打开杂项界面 
#define Weigh_opened   Micil_opened +1//打开称重界面 
#define Test_In_Out_opened  Weigh_opened +1//打开输入输出测试界面界面

#define Tare_button  Test_In_Out_opened +1//去皮按钮·
#define zero_button  Tare_button +1//置零按钮
char  Tare_button_pressed,zero_button_pressed;

#define Cal_zero_confirm_key  zero_button +1//标定零位确认键·
#define Cal_weight_confirm_key  Cal_zero_confirm_key +1//标定砝码确认键
#define Close_zeroing_err_mesgbox Cal_weight_confirm_key +1//置零错误信息框关闭按钮
#define Close_cal_ing_err_mesgbox Close_zeroing_err_mesgbox +1//标定错误信息框关闭按钮
void Recover_para(void);

 

char Cal_zero_confirm_key_pressed,Cal_weight_confirm_key_pressed;
int q_num;
unsigned char supervision_ad_com;

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

char Modbus_0x10_Value[30],Modbus_value_count,Modbus_byte_count;

void TestY_output(int Modbus_Address );
char face_in_out_been_opened;
char Modbus_monitor_x;
char Modbus_function;
int  Modbus_Address,Modbus_value,Modbus_CRC_CODE;
unsigned char Modbus_Data[1000];
int Modbus_Counter;
extern char Modbus_input_buffer[2],Modbus_output_buffer[2];
char Feed_signal_buffer[2];//反馈信号，如 去皮 置零
char waitting_for_KLTT_PARA;
extern union Modbus_packed    
{    
long weight_long; 
unsigned char long_weight[4];   
}Modbus_weight;

union Modbus_packed_1    
{    
long target_totall_pulse; 
unsigned char pulse[4];   
}Modbus_pulse;

union Modbus_packed_2    
{    
long target_value; 
unsigned char target[4];   
}Modbus_target;


char KLTT_tare,KLTT_zero;
char Cal_been_opened;
extern int image_Pre_feed,system_indicator;

extern int image_Pulse_howmany_package,image_Even_howmany_package_Pre_process;
extern int image_Tare_howmany_package;

extern void recive_AD_code(unsigned char Data);
char elimate_USART_DE[6];

char Pc_recived;
extern unsigned char status[6];


extern int shut_initial_mid_slow_prefeed_timer[4],set_shut_initial_mid_slow_prefeed_timer;  //*2017-10-28am*
void return_data(unsigned char *comm3_reply_point);
void return_data_xx(unsigned char *comm3_reply_point);
extern void PC_comm3_command(unsigned char Data);

void Reply_pc_inquier(void);
unsigned char Datax;
void Write_image_para(char i);
/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
unsigned int mtb_crc_calc( unsigned char *crc_str, char count)
{
	  unsigned char i,j;
	  unsigned int CrcValue, temp;
	
  	CrcValue = 0xffff;
	
	  for(i=0; i<count ;i++,crc_str++)  
	    { 
	     	temp = (unsigned int)*crc_str & 0x0FF;
	     	CrcValue ^= temp ;
				
	     	for (j=0; j<8; j++)
	       	{
	        		if (CrcValue & 0x0001)
			          	CrcValue =(CrcValue >> 1) ^ 0x0A001;
			        else
				          CrcValue = CrcValue >> 1;	
	      	}
	    }
  	return CrcValue;
}

void USART1_REMAP_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
		/* 配置串口1 （USART1） 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	/*串口GPIO端口配置*/
  /* 配置串口1 （USART1 Tx (PB.6)）*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;// 推挽输出 GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* 配置串口1 USART1 Rx (PB.7)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
		/* 串口1工作模式（USART1 mode）配置 */
	USART_InitStructure.USART_BaudRate = 9600*2;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);//使能串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//+++++++++++++++++++++++++

}

void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
		/* 配置串口1 （USART1） 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	/*串口GPIO端口配置*/
  /* 配置串口1 （USART1 Tx (PA.09)）*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;// 推挽输出 GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* 配置串口1 USART1 Rx (PA.10)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
		/* 串口1工作模式（USART1 mode）配置 */
	USART_InitStructure.USART_BaudRate = 9600*2;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);//使能串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//+++++++++++++++++++++++++

}
void USART3_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
 
/* 使能 USART3 时钟*/
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
 
 
/* USART3 使用IO端口配置 */    
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOB, &GPIO_InitStructure);    
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA
 
/* USART3 工作模式配置 */
USART_InitStructure.USART_BaudRate = 9600; //波特率设置：115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位数设置：8位
USART_InitStructure.USART_StopBits = USART_StopBits_1;  //停止位设置：1位
USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制模式设置：没有使能
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
//USART_InitStructure.USART_Mode = USART_Mode_Rx;//接收使能

USART_Init(USART3, &USART_InitStructure);  //初始化USART3

/*使能串口3的接收中断*/
USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
USART_Cmd(USART3, ENABLE);// USART3 Enable
}

void USART2_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
 
/* 使能 USART2 时钟*/
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
 
 
/* USART2 使用IO端口配置 */    
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);    
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
 
/* USART2 工作模式配置 */
USART_InitStructure.USART_BaudRate =4800;//9600;// 9600*2; //波特率设置：115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位数设置：8位
USART_InitStructure.USART_StopBits = USART_StopBits_1;  //停止位设置：1位
USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制模式设置：没有使能
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
USART_Init(USART2, &USART_InitStructure);  //初始化USART2

/*使能串口2的发送和接收中断*/
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  
USART_Cmd(USART2, ENABLE);// USART2 Enable
}

void UART4_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
 
/* 使能 UART4 时钟*/
RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
 
 
/* UART4 使用IO端口配置 */    
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOC, &GPIO_InitStructure);    
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
GPIO_Init(GPIOC, &GPIO_InitStructure);   //初始化GPIOA
 
/* UART4 工作模式配置 */
USART_InitStructure.USART_BaudRate = 9600; //波特率设置：115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位数设置：8位
USART_InitStructure.USART_StopBits = USART_StopBits_1;  //停止位设置：1位
USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制模式设置：没有使能
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
USART_Init(UART4, &USART_InitStructure);  //初始化UART4
 
/*使能串口3的发送和接收中断*/
USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);  
USART_Cmd(UART4, ENABLE);// UART4 Enable
}

void UART5_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
 
/* 使能 UART5 时钟*/
RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
 
 
/* UART5 使用IO端口配置 */    
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOC, &GPIO_InitStructure);    
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
GPIO_Init(GPIOD, &GPIO_InitStructure);   //初始化GPIOA
 
/* UART5 工作模式配置 */
USART_InitStructure.USART_BaudRate = 9600*2; //波特率设置：115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位数设置：8位
USART_InitStructure.USART_StopBits = USART_StopBits_1;  //停止位设置：1位
USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制模式设置：没有使能
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
USART_Init(UART5, &USART_InitStructure);  //初始化UART5
 
/*使能串口3的发送和接收中断*/
USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);  
USART_Cmd(UART5, ENABLE);// UART5 Enable
}

void initi_usart2_control_pin(void)
{
/* control pins for USART2_485 Comunication RE:PA5 DE:PA1 CT:(无线)PA7  RT:(无线)PC3*/
	/*初始化输出--推挽*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_1 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
}
 /*********************************************************************/
//中断配置
void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;
 NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000); 

// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     //先占优先权2，从优先级2位
// NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;// USART1_IRQn;    //开串口中断1
// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //指定抢占优先级别1
// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //指定相应优先级别0
// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// NVIC_Init(&NVIC_InitStructure);

 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     //先占优先权2，从优先级2位
 NVIC_InitStructure.NVIC_IRQChannel =USART2_IRQn;   //开串口中断1
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //指定抢占优先级别1
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //指定相应优先级别0
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);

 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     //先占优先权2，从优先级2位
 NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;   //开串口中断1
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //指定抢占优先级别1
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //指定相应优先级别0
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);

// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     //先占优先权2，从优先级2位
// NVIC_InitStructure.NVIC_IRQChannel =52;//UART4_IRQn;// USART1_IRQn;    //开串口中断1
// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //指定抢占优先级别1
// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //指定相应优先级别0
// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// NVIC_Init(&NVIC_InitStructure);
//
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     //先占优先权2，从优先级2位
// NVIC_InitStructure.NVIC_IRQChannel =53;//UART5_IRQn;// USART1_IRQn;    //开串口中断1
// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;   //指定抢占优先级别1
// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //指定相应优先级别0
// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// NVIC_Init(&NVIC_InitStructure);

// NVIC_InitStructure.NVIC_IRQChannel =DMA2_Channel5_IRQn;

////NVIC_InitTypeDef NVIC_InitStructure;
///*设置NVIC中断分组2:2位抢占优先级，2位响应优先级*/
//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
///*使能按键所在的外部中断通道*/
//NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
///*设置抢占优先级，抢占优先级设为2*/	
//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;// 0x02;	
///*设置子优先级，子优先级设为1*/
//NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;// 0x01;		
///*使能外部中断通*/
//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
/*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器*/		
//NVIC_Init(&NVIC_InitStructure); 
//

}
void EXTI_NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	
/*使能按键所在的外部中断通道*/
//NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
/*设置抢占优先级，抢占优先级设为2*/	
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;// 0x02;	
/*设置子优先级，子优先级设为1*/
NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0x01;		
/*使能外部中断通*/
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
/*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器*/	

//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
///*使能按键所在的外部中断通道*/
//NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
///*设置抢占优先级，抢占优先级设为2*/	
//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
///*设置子优先级，子优先级设为1*/
//NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x01;		
///*使能外部中断通*/
//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	
//NVIC_Init(&NVIC_InitStructure); 
 NVIC_Init(&NVIC_InitStructure);
 }




/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

int read_com1(void)
{
  
   while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
   {
   }
  
   USART_ClearFlag(USART1,USART_FLAG_RXNE);
   return (USART_ReceiveData(USART1));
}
//注释：
//fgetc 和 fgetc是c语言的标准函数（形参是标准的） FILE *f 是文件指针，
//具备文件系统的操作系统有用。对于简单的无文件系统的嵌入式系统无用。
void USART1_Process(void)
{
	unsigned char Data,temps[3];
	int px,return_crc;
	u8 *p,i,len,byte_count;
 #define recive_para 100

// Pre_feed++;//=1001;//6 参数提前量	
// Weighting_T+=2;
// Weighting_package+=4;
// Weighting_even+=3;
//  if(q_num<15001)q_num+=1;	,x00

// if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
// {x00=9;}

	 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{	
//		Data = (unsigned char)USART_ReceiveData(USART1);
//		Modbus_Data[Modbus_Counter]=Data;

		switch (Modbus_Counter)
		{
			case 0:
				if(Data==0xc8)//slave
				{
					Modbus_monitor_x=50;//超时监视
					Modbus_Counter++;
				}
			break;
			case 1:
				 if((Data!=0x03)&&(Data!=0x05)&&(Data!=0x10))
				 {Modbus_Counter=0;break;}
			     Modbus_function=Data;
				 Modbus_Counter++;
			break;
			case 2:
			     Modbus_Address=Data;
				 Modbus_Address<<=8;
				 Modbus_Counter++;
			break;
			case 3:
			     Modbus_Address|=Data;
				 Modbus_Counter++; //addr
			break;
			case 4:  
			     Modbus_value=Data;
				 Modbus_value<<=8;
				 Modbus_Counter++;	
			break;
			case 5:
			     Modbus_value|=Data;
				 if(Modbus_function==0x10)
				   {Modbus_Counter=8;}
				 else
				   {Modbus_Counter++;} //count of reg
			break;
			case 6:
			     Modbus_CRC_CODE=Data;
				 Modbus_CRC_CODE<<=8;
				 Modbus_Counter++;
			break;
			case 7:
			      Modbus_CRC_CODE|=Data;
                  switch (Modbus_function)
					  {
					  	case 5://fun=0x05,CRC and store state and replay
								if (Modbus_Address==Tare_button)// 去皮按钮
								{ 
									if(Modbus_value==0xff00)//去皮按钮按下
									{Tare_button_pressed=1;
//									 Feed_signal_buffer[1]|=0x80;	//反馈信号，如 去皮 
									 KLTT_tare=1;
									 //Feed_signal_buffer[0]|=0xff;
								    }
									else
									{Tare_button_pressed=0;
//									 Feed_signal_buffer[1]|=0x80;	//反馈信号，如 去皮 
//									 Feed_signal_buffer[1]&=0x7f;
									 KLTT_tare=0;
									}
								}

								if (Modbus_Address==zero_button)// 置零按钮
								{
									if(Modbus_value==0xff00)//置零按钮按下
									{zero_button_pressed=1;
//									 Feed_signal_buffer[1]|=0x40;	//反馈信号，如 置零	
									 KLTT_zero=1;								
									}
									else
									{zero_button_pressed=0;
//									 Feed_signal_buffer[1]&=0xbf;
									 KLTT_zero=0;
									}
								}

								if (Modbus_Address==Cal_zero_confirm_key)//标定零位确认键
								{
									if(Modbus_value==0xff00)//标定零位确认键按钮按下
									{Cal_zero_confirm_key_pressed=1;
									Cal_output;
									}
									else
									{Cal_zero_confirm_key_pressed=0;
									}
								}

								if (Modbus_Address==Cal_weight_confirm_key)//标定砝码确认键
								{
									if(Modbus_value==0xff00)//标定砝码确认键按钮按下
									{Cal_weight_confirm_key_pressed=1;
									 Un_Cal_output;
									}
									else
									{Cal_weight_confirm_key_pressed=0;
									}
								}

								
 								if (Modbus_Address==Cal_opened)// 
								{
									if(Modbus_value==0xff00)//open 标定界面被打开
									{Cal_been_opened=1;}
									else
									{Cal_been_opened=0;}
								}

  
  								if (Modbus_Address==Close_zeroing_err_mesgbox)// 
								{
									if(Modbus_value==0xff00)////置零错误信息框关闭按钮
									{Feed_signal_buffer[0]&=0xfe;}
								}


  								if (Modbus_Address==Close_cal_ing_err_mesgbox)// 
								{
									if(Modbus_value==0xff00)////标定错误信息框关闭按钮
									{Feed_signal_buffer[0]&=0xfd;}
								}


								if (Modbus_Address==Test_In_Out_opened)// 
								{
									if(Modbus_value==0xff00)//open 测试输入输出界面被打开
									{face_in_out_been_opened=1;}
									else
									{face_in_out_been_opened=0;}
								}
								
								if(face_in_out_been_opened)
									{TestY_output(Modbus_Address);}
								Modbus_Data[++Modbus_Counter]=0;
								Modbus_function=0;//if CRC ok 
								Modbus_Counter=0;
								Modbus_Data[0]=0;
							break;
						case 3://fun=0x03,CRC and replay 
								len=6;
								px=CRC16(Modbus_Data,len);
								p=Modbus_Data;
								byte_count=Modbus_value;//count of reg
								byte_count*=2;
								if(px==Modbus_CRC_CODE)
									{
									  *(p++)=0xc8;
									  *(p++)=0x03;
									  *(p++)=byte_count;
									  for(i=0;i<byte_count;i++)
									   {
		                                  switch (i)
		                                       {
		                                        case 0:	//输入
													  *(p++)=Modbus_input_buffer[1];//高在先 0x83;	  //contents
		                                            break;
		                                        case 1:
													  *(p++)=Modbus_input_buffer[0];//0xc7;	  //contents
		                                            break;
		                                        case 2:	//输出
													  *(p++)=Modbus_output_buffer[1];//高在先 0xc5;	  //contents
		                                            break;
		                                        case 3:
													  *(p++)=Modbus_output_buffer[0];//0xb6;	  //contents
		                                            break;

		                                        case 4:	//重量
													  *(p++)=Modbus_weight.long_weight[3];// 高在先 
		                                            break;
		                                        case 5:
													  *(p++)=Modbus_weight.long_weight[2];
		                                            break;
		                                        case 6:
													  *(p++)=Modbus_weight.long_weight[1];
		                                            break;
		                                        case 7:
													  *(p++)=Modbus_weight.long_weight[0];
		                                            break;

//		                                        case 8:	//参数定量的总脉冲数
//													  *(p++)=Modbus_pulse.pulse[3];
//		                                            break;
//		                                        case 9:
//													  *(p++)=Modbus_pulse.pulse[2];
//		                                            break;
//		                                        case 10:
//													  *(p++)=Modbus_pulse.pulse[1];
//		                                            break;
//		                                        case 11:
//													  *(p++)=Modbus_pulse.pulse[0];
//		                                            break;
//
//		                                        case 12:	//参数定量
//													  *(p++)=Modbus_target.target[3];
//		                                            break;
//		                                        case 13:
//													  *(p++)=Modbus_target.target[2];
//		                                            break;
//		                                        case 14:
//													  *(p++)=Modbus_target.target[1];
//		                                            break;
//		                                        case 15:
//													  *(p++)=Modbus_target.target[0];
//		                                            break;
//												//1.通讯数据组
//		                                        case 16:	//参数波特率关联号
//													  *(p++)=(char)ID_budrate>>8;
//		                                            break;
//		                                        case 17:
//													  *(p++)=(char)ID_budrate;
//		                                            break;
//		                                        case 18:   //参数通讯地址关联号
//													  *(p++)=(char)Comm_Addr>>8;
//		                                            break;
//		                                        case 19:
//													  *(p++)=(char)Comm_Addr;
//		                                            break;
//												//2.置零数据组
//		                                        case 20:	//参数开机置零范围
//													  *(p++)=(char)Initial_zero_range>>8;
//		                                            break;
//		                                        case 21:
//													  *(p++)=(char)Initial_zero_range;
//		                                            break;
//		                                        case 22:   //参数手动置零范围
//													  *(p++)=(char)Munal_zero_range>>8;
//		                                            break;
//		                                        case 23:
//													  *(p++)=(char)Munal_zero_range;
//		                                            break;
//		                                        case 24:   //参数零跟踪范围
//													  *(p++)=(char)Trace_zero_range>>8;
//		                                            break;
//		                                        case 25:
//													  *(p++)=(char)Trace_zero_range;
//		                                            break;
//
//											   //____________________________________
////3.延时数据组
// 		                                        case 26:	//参数去皮延时输出
//													  *(p++)=Delay_tare>>8;
//		                                            break;
//		                                        case 27:
//													  *(p++)=Delay_tare;
//		                                            break;
//		                                        case 28:   //参数中加延时
//													  *(p++)=Delay_Mid_feed>>8;
//		                                            break;
//		                                        case 29:
//													  *(p++)=Delay_Mid_feed;
//		                                            break;
//		                                        case 30:   //参数提前延时
//													  *(p++)=Delay_Pre_process>>8;
//		                                            break;
//		                                        case 31:
//													  *(p++)=Delay_Pre_process;
//		                                            break;
//
//		                                        case 32:   //参数下一循环延时
//													  *(p++)=Delay_Next_loop>>8;
//		                                            break;
//		                                        case 33:
//													  *(p++)=Delay_Next_loop;
//		                                            break;
//
//		                                        case 34:	//参数慢加监视上限时间
//													  *(p++)=Time_supvisor_Slow_feed_up_limt>>8;
//		                                            break;
//		                                        case 35:
//													  *(p++)=Time_supvisor_Slow_feed_up_limt;
//		                                            break;
//
//		                                        case 36:   //参数慢加监视下限时间
//													  *(p++)=Time_supvisor_Slow_feed_low_limt>>8;
//		                                            break;
//		                                        case 37:
//													  *(p++)=Time_supvisor_Slow_feed_low_limt;
//		                                            break;
//
//
//		                                        case 38:   //参数多少袋输出脉冲
//													  *(p++)=Pulse_howmany_package>>8;
//		                                            break;
//		                                        case 39:
//													  *(p++)=Pulse_howmany_package;
//		                                            break;
//
// 		                                        case 40:	//参数多少袋去皮一次
//													  *(p++)=Tare_howmany_package>>8;
//		                                            break;
//		                                        case 41:
//													  *(p++)=Tare_howmany_package;
//		                                            break;
//
//		                                        case 42:   //参数收到x次信号设定
//													  *(p++)=Recive_x_signals>>8;
//		                                            break;
//		                                        case 43:
//													  *(p++)=Recive_x_signals;
//		                                            break;
//
//
//		                                        case 44:   //参数收不到x次信号设定限
//													  *(p++)=no_recive_x_signals>>8;
//		                                            break;
//		                                        case 45:
//													  *(p++)=no_recive_x_signals;
//		                                            break;
//
// 		                                        case 46:	//参数多少袋平均修正一次提前量
//													  *(p++)=Even_howmany_package_Pre_process>>8;
//		                                            break;
//		                                        case 47:
//													  *(p++)=Even_howmany_package_Pre_process;
//		                                            break;
//
//
//		                                        case 48:   //参数慢加延时
//													  *(p++)=Delay_slow_feed>>8;
//		                                            break;
//		                                        case 49:
//													  *(p++)=Delay_slow_feed;
//		                                            break;
//
//		                                        case 50:   //参数初加延时
//													  *(p++)=Delay_initial_feed>>8;
//		                                            break;
//		                                        case 51:
//													  *(p++)=Delay_initial_feed;
//		                                            break;
//
//											   //*************************************
////4.控制数据组
// 		                                        case 52:   //1 参数慢加修正上限
//													  *(p++)=Slow_feed_modify_uper_limt>>8;
//		                                            break;
//		                                        case 53:
//													  *(p++)=Slow_feed_modify_uper_limt;
//		                                            break;
// 		                                        case 54:   //2 参数慢加修正下限
//													  *(p++)=Slow_feed_modify_down_limt>>8;
//		                                            break;
//		                                        case 55:
//													  *(p++)=Slow_feed_modify_down_limt;
//		                                            break;
// 		                                        case 56:   //3 参数提前量修正上限
//													  *(p++)=Pre_feed_modify_uper_limt>>8;
//		                                            break;
//		                                        case 57:
//													  *(p++)=Pre_feed_modify_uper_limt;
//		                                            break;
// 		                                        case 58:   //4 参数提前量修正下限
//													  *(p++)=Pre_feed_modify_down_limt>>8;
//		                                            break;
//		                                        case 59:
//													  *(p++)=Pre_feed_modify_down_limt;
//		                                            break;
// 		                                        case 60:   //5 参数慢加量
//													  *(p++)=Slow_feed>>8;
//		                                            break;
//		                                        case 61:
//													  *(p++)=Slow_feed;
//		                                            break;
// 		                                        case 62:   //6 参数提前量
//													  *(p++)=Pre_feed>>8;
//		                                            break;
//		                                        case 63:
//													  *(p++)=Pre_feed;
//		                                            break;
// 		                                        case 64:   //7 参数中加值
//													  *(p++)=Mid_feed>>8;
//		                                            break;
//		                                        case 65:
//													  *(p++)=Mid_feed;
//		                                            break;
// 		                                        case 66:   //8 参数允差
//													  *(p++)=Dlet_error>>8;
//		                                            break;
//		                                        case 67:
//													  *(p++)=Dlet_error;
//		                                            break;
// 		                                        case 68:   //9 参数定值
//													  *(p++)=Target>>8;
//		                                            break;
//		                                        case 69:
//													  *(p++)=Target;
//		                                            break;
//
// 		                                        case 70:   //a 参数定量上限
//													  *(p++)=Target_uper_limt>>8;
//		                                            break;
//		                                        case 71:
//													  *(p++)=Target_uper_limt;
//		                                            break;
// 		                                        case 72:   //b  参数定量下限
//													  *(p++)=Target_down_limt>>8;
//		                                            break;
//		                                        case 73:
//													  *(p++)=Target_down_limt;
//		                                            break;
// 		                                        case 74:   //c  参数零区
//													  *(p++)=zero_range>>8;
//		                                            break;
//		                                        case 75:
//													  *(p++)=zero_range;
//		                                            break;
//  		                                        case 76:   //d 参数慢加修正量
//													  *(p++)=Slow_feed_modify>>8;
//		                                            break;
//		                                        case 77:
//													  *(p++)=Slow_feed_modify;
//		                                            break;
//
//
//  		                                        case 78:   //d 参数初上电值
//													  *(p++)=Initial_power_value>>8;
//		                                            break;
//		                                        case 79:
//													  *(p++)=Initial_power_value;
//		                                            break;
//  		                                        case 80:   //d 参数初加量
//													  *(p++)=Initial_feed_value>>8;
//		                                            break;
//		                                        case 81:
//													  *(p++)=Initial_feed_value;
//		                                            break;
//  		                                        case 82:   //d 参数随机设定值
//													  *(p++)=Random_setting_value>>8;
//		                                            break;
//		                                        case 83:
//													  *(p++)=Random_setting_value;
//		                                            break;
////5.修正数据组
//  		                                        case 84:   //参数用户标定关联号
//													  *(p++)=ID_user_cal>>8;
//		                                            break;
//		                                        case 85:
//													  *(p++)=ID_user_cal;
//		                                            break;
//  		                                        case 86:   //参数慢加修正标记关联号
//													  *(p++)=ID_slow_feed_modify>>8;
//		                                            break;
//		                                        case 87:
//													  *(p++)=ID_slow_feed_modify;
//		                                            break;
//
//  		                                        case 88:   //参数提前修正标记关联号
//													  *(p++)=ID_pre_feed_modify>>8;
//		                                            break;
//		                                        case 89:
//													  *(p++)=ID_pre_feed_modify;
//		                                            break;
//
//  		                                        case 90:   //参数运行模式关联号
//													  *(p++)=ID_run_mode>>8;
//		                                            break;
//		                                        case 91:
//													  *(p++)=ID_run_mode;
//		                                            break;
//
//
////7.标定数据组
//  		                                        case 92:   //标定分度数关联号
//													  *(p++)=Cal_ID_division>>8;
//		                                            break;
//		                                        case 93:
//													  *(p++)=Cal_ID_division;
//		                                            break;
//
//		                                        case 94:	//标定用户砝码重量
//													  *(p++)=Modbus_Cal_user_weight.user_weight[3];
//		                                            break;
//		                                        case 95:
//													  *(p++)=Modbus_Cal_user_weight.user_weight[2];
//		                                            break;
//		                                        case 96:
//													  *(p++)=Modbus_Cal_user_weight.user_weight[1];
//		                                            break;
//		                                        case 97:
//													  *(p++)=Modbus_Cal_user_weight.user_weight[0];
//		                                            break;
//
//		                                        case 98:	//标定砝码重量
//													  *(p++)=Modbus_Cal_weight.add_weight[3];
//		                                            break;
//		                                        case 99:
//													  *(p++)=Modbus_Cal_weight.add_weight[2];
//		                                            break;
//		                                        case 100:
//													  *(p++)=Modbus_Cal_weight.add_weight[1];
//		                                            break;
//		                                        case 101:
//													  *(p++)=Modbus_Cal_weight.add_weight[0];
//		                                            break;
//
////8.滤波数据组：
//  		                                        case 102:   //参数滤波范围0
//													  *(p++)=Range_Filter0>>8;
//		                                            break;
//		                                        case 103:
//													  *(p++)=Range_Filter0;
//		                                            break;
//  		                                        case 104:   //参数滤波范围1
//													  *(p++)=Range_Filter1>>8;
//		                                            break;
//		                                        case 105:
//													  *(p++)=Range_Filter1;
//		                                            break;
//
//  		                                        case 106:   //参数滤波范围2
//													  *(p++)=Range_Filter2>>8;
//		                                            break;
//		                                        case 107:
//													  *(p++)=Range_Filter2;
//		                                            break;
//
//  		                                        case 108:   //参数滤波范围3
//													  *(p++)=Range_Filter3>>8;
//		                                            break;
//		                                        case 109:
//													  *(p++)=Range_Filter3;
//		                                            break;
//
//  		                                        case 110:   //参数滤波范围4
//													  *(p++)=Range_Filter4>>8;
//		                                            break;
//		                                        case 111:
//													  *(p++)=Range_Filter4;
//		                                            break;
//												/******************/
//					   		                   case 112:   //参数滤波强度0
//													  *(p++)=Strong_Filter0>>8;
//		                                            break;
//		                                        case 113:
//													  *(p++)=Strong_Filter0;
//		                                            break;
//  		                                        case 114:   //参数滤波强度1
//													  *(p++)=Strong_Filter1>>8;
//		                                            break;
//		                                        case 115:
//													  *(p++)=Strong_Filter1;
//		                                            break;
//
//  		                                        case 116:   //参数滤波强度2
//													  *(p++)=Strong_Filter2>>8;
//		                                            break;
//		                                        case 117:
//													  *(p++)=Strong_Filter2;
//		                                            break;
//
//  		                                        case 118:   //参数滤波强度3
//													  *(p++)=Strong_Filter3>>8;
//		                                            break;
//		                                        case 119:
//													  *(p++)=Strong_Filter3;
//		                                            break;
//
//  		                                        case 120:   //参数滤波强度4
//													  *(p++)=Strong_Filter4>>8;
//		                                            break;
//		                                        case 121:
//													  *(p++)=Strong_Filter4;
//		                                            break;
////反馈信号，如 去皮 置零
// 		                                        case 122:	//反馈信号，如 去皮 置零
//													  *(p++)=Feed_signal_buffer[1];//高在先 0x83;	  //contents
//		                                            break;
//		                                        case 123:
//													  *(p++)=Feed_signal_buffer[0];//0xc7;	  //contents
//		                                            break;
//
////称重_吨位，袋数，平均值
////int Weighting_T,Weighting_package,Weighting_even;
//  		                                        case 124:   //称重_吨位
//													  *(p++)=Weighting_T>>8;
//		                                            break;
//		                                        case 125:
//													  *(p++)=Weighting_T;
//		                                            break;
//
//  		                                        case 126:   //称重_袋数
//													  *(p++)=Weighting_package>>8;
//		                                            break;
//		                                        case 127:
//													  *(p++)=Weighting_package;
//		                                            break;
//
//  		                                        case 128:   //称重_平均值
//													  *(p++)=Weighting_even>>8;
//		                                            break;
//		                                        case 129:
//													  *(p++)=Weighting_even;
//		                                            break;

//反馈信号，如 去皮 置零
 		                                        case 8:	//反馈信号，如 去皮 置零
													  *(p++)=Feed_signal_buffer[1];//高在先 0x83;	  //contents
		                                            break;
		                                        case 9:
													  *(p++)=Feed_signal_buffer[0];//0xc7;	  //contents
		                                            break;

//称重_吨位，袋数，平均值
//int Weighting_T,Weighting_package,Weighting_even;
  		                                        case 10:   //称重_吨位
													  *(p++)=Weighting_T>>8;
		                                            break;
		                                        case 11:
													  *(p++)=Weighting_T;
		                                            break;

  		                                        case 12:   //称重_袋数
													  *(p++)=Weighting_package>>8;
		                                            break;
		                                        case 13:
													  *(p++)=Weighting_package;
		                                            break;

  		                                        case 14:   //称重_平均值
													  *(p++)=Weighting_even>>8;
		                                            break;
		                                        case 15:
													  *(p++)=Weighting_even;
		                                            break;

  		                                        case 16:   //回读当前提前量
													  *(p++)=image_Pre_feed>>8;
		                                            break;
		                                        case 17:
													  *(p++)=image_Pre_feed;
		                                            break;

   		                                        case 18:   //system_indicator
													  *(p++)=system_indicator>>8;
		                                            break;
		                                        case 19:
													  *(p++)=system_indicator;
		                                            break;


											   //*************************************
		                                        default:
		                                            break;
		                                      }
									   }
									   len=3+ byte_count; //length of  CRC line  3:0xc8 0x03 byte_counter
								        px=CRC16((unsigned char *)Modbus_Data,len);
										*(p++)=px>>8;//temps[0];	
										*(p++)=px&0x0ff;//temps[1];
										*(p++)=0;//end
										p=Modbus_Data;
										len+=2;
											DMA_Configuration((char *)Modbus_Data,len);
										    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
										    DMA_Cmd(DMA1_Channel4, ENABLE);
											while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);
//											GPIO_WriteBit(GPIOE, GPIO_Pin_6, (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_6))));//第一LED:通讯
									}
										Modbus_function=0;//if CRC ok 
										Modbus_Counter=0;
										Modbus_Data[0]=0;
							break;
						default:
							   Modbus_Data[0]=0;
						       Modbus_function=0;//if CRC ok 
							   Modbus_Counter=0;
						break;
					}
			 break;
		     case 8:
			        Modbus_byte_count=Data;
					Modbus_Data[6]=Data;
					Modbus_value_count=0;
	                Modbus_Counter++;
			  break;
		     case 9:
//					if(!Modbus_byte_count)Modbus_Counter++;
//					Modbus_Data[7+Modbus_value_count]=Data;
//					Modbus_0x10_Value[Modbus_value_count++]=Data;
//					if(Modbus_value_count<Modbus_byte_count)break;
//					Modbus_Counter++;//参数定量的总脉冲数
					if(Modbus_Address==1503) //1003:Modbus地址	 1003  FF_Target_pulse
					{
						Modbus_pulse.pulse[3]=0;//Modbus_0x10_Value[0];
						Modbus_pulse.pulse[2]=0;//Modbus_0x10_Value[1];
						Modbus_pulse.pulse[1]=Modbus_0x10_Value[0];
						Modbus_pulse.pulse[0]=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Target_pulse,(u16*)&Modbus_pulse.target_totall_pulse,4);
					}
//					if(Modbus_Address==1505)//1005:Modbus地址
//					{
//						Modbus_target.target[3]=Modbus_0x10_Value[0];
//						Modbus_target.target[2]=Modbus_0x10_Value[1];
//						Modbus_target.target[1]=Modbus_0x10_Value[2];
//						Modbus_target.target[0]=Modbus_0x10_Value[3];
//					}
//				    target_value  Modbus_pulse
//1.通讯数据组
					if(Modbus_Address==comm_bud_rate)//
					{
						ID_budrate=Modbus_0x10_Value[0];//参数波特率关联号
						ID_budrate<<=8;
						ID_budrate|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_ID_budrate,(u16*)&ID_budrate,4);
					}
					if(Modbus_Address==comm_address)//
					{
						Comm_Addr=Modbus_0x10_Value[0];//参数通讯地址关联号
						Comm_Addr<<=8;
						Comm_Addr|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Comm_Addr,(u16*)&Comm_Addr,4);
					}

//2.置零数据组
					if(Modbus_Address==Initial_zero_range_address)//
					{
						Initial_zero_range=Modbus_0x10_Value[0];//参数开机置零范围
						Initial_zero_range<<=8;
						Initial_zero_range|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Initial_zero_range,(u16*)&Initial_zero_range,4);
					}
					if(Modbus_Address==Munal_zero_range_address)//
					{
						Munal_zero_range=Modbus_0x10_Value[0];//参数手动置零范围
						Munal_zero_range<<=8;
						Munal_zero_range|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Munal_zero_range,(u16*)&Munal_zero_range,4);
					}
					if(Modbus_Address==Trace_zero_range_address)//
					{
						Trace_zero_range=Modbus_0x10_Value[0];//参数零跟踪范围
						Trace_zero_range<<=8;
						Trace_zero_range|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Trace_zero_range,(u16*)&Trace_zero_range,4);
					}

//3.延时数据组
//int Delay_tare;//参数去皮延时输出
					if(Modbus_Address==Delay_tare_address)//
					{
						Delay_tare=Modbus_0x10_Value[0];//参数去皮延时输出
						Delay_tare<<=8;
						Delay_tare|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Delay_tare,(u16*)&Delay_tare,4);
//						STMFLASH_Write(FF_,(u16*)&,4);

					}

//int Delay_Mid_feed;//参数中加延时
					if(Modbus_Address==Delay_Mid_feed_address)//
					{
						Delay_Mid_feed=Modbus_0x10_Value[0];//参数中加延时
						Delay_Mid_feed<<=8;
						Delay_Mid_feed|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Delay_Mid_feed,(u16*)&Delay_Mid_feed,4);
					}

//int Delay_Pre_process;//参数提前延时
					if(Modbus_Address==Delay_Pre_process_address)//
					{
						Delay_Pre_process=Modbus_0x10_Value[0];//参数提前延时
						Delay_Pre_process<<=8;
						Delay_Pre_process|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Delay_Pre_process,(u16*)&Delay_Pre_process,4);
					}

//int Delay_Next_loop;//参数下一循环延时
					if(Modbus_Address==Delay_Next_loop_address)//
					{
						Delay_Next_loop=Modbus_0x10_Value[0];//参数零跟踪范围
						Delay_Next_loop<<=8;
						Delay_Next_loop|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Delay_Next_loop,(u16*)&Delay_Next_loop,4);
					}

//int Time_supvisor_Slow_feed_up_limt;//参数慢加监视上限时间
					if(Modbus_Address==Time_supvisor_Slow_feed_up_limt_address)//
					{
						Time_supvisor_Slow_feed_up_limt=Modbus_0x10_Value[0];//参数慢加监视上限时间
						Time_supvisor_Slow_feed_up_limt<<=8;
						Time_supvisor_Slow_feed_up_limt|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Time_supvisor_Slow_feed_up_limt,(u16*)&Time_supvisor_Slow_feed_up_limt,4);
					}

//int Time_supvisor_Slow_feed_low_limt;//参数慢加监视下限时间
					if(Modbus_Address==Time_supvisor_Slow_feed_low_limt_address)//
					{
						Time_supvisor_Slow_feed_low_limt=Modbus_0x10_Value[0];//参数慢加监视下限时间
						Time_supvisor_Slow_feed_low_limt<<=8;
						Time_supvisor_Slow_feed_low_limt|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Time_supvisor_Slow_feed_low_limt,(u16*)&Time_supvisor_Slow_feed_low_limt,4);
					}

//int Pulse_howmany_package;//参数多少袋输出脉冲
					if(Modbus_Address==Pulse_howmany_package_address)//
					{
						Pulse_howmany_package=Modbus_0x10_Value[0];//参数多少袋输出脉冲
						Pulse_howmany_package<<=8;
						Pulse_howmany_package|=Modbus_0x10_Value[1];
						image_Pulse_howmany_package=Pulse_howmany_package;//初始化工作
						STMFLASH_Write(FF_Pulse_howmany_package,(u16*)&Pulse_howmany_package,4);
						STMFLASH_Write(FF_image_Pulse_howmany_package,(u16*)&image_Pulse_howmany_package,4);
					}

//int Tare_howmany_package;//参数多少袋去皮一次
					if(Modbus_Address==Tare_howmany_package_address)//
					{
						Tare_howmany_package=Modbus_0x10_Value[0];//参数多少袋去皮一次
						Tare_howmany_package<<=8;
						Tare_howmany_package|=Modbus_0x10_Value[1];
						image_Tare_howmany_package=Tare_howmany_package;//初始化工作
						STMFLASH_Write(FF_Tare_howmany_package,(u16*)&Tare_howmany_package,4);
						STMFLASH_Write(FF_image_Tare_howmany_package,(u16*)&image_Tare_howmany_package,4);
					}

//int Recive_x_signals;//参数收到x次信号设定
					if(Modbus_Address==Recive_x_signals_address)//
					{
						Recive_x_signals=Modbus_0x10_Value[0];//参数收到x次信号设定
						Recive_x_signals<<=8;
						Recive_x_signals|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Recive_x_signals,(u16*)&Recive_x_signals,4);
					}

//int no_recive_x_signals;//参数收不到x次信号设定限
					if(Modbus_Address==no_recive_x_signals_address)//
					{
						no_recive_x_signals=Modbus_0x10_Value[0];//参数收不到x次信号设定限
						no_recive_x_signals<<=8;
						no_recive_x_signals|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_no_recive_x_signals,(u16*)&no_recive_x_signals,4);
					}

//int Even_howmany_package_Pre_process;//参数多少袋平均修正一次提前量
					if(Modbus_Address==Even_howmany_package_Pre_process_address)//
					{
						Even_howmany_package_Pre_process=Modbus_0x10_Value[0];//参数多少袋平均修正一次提前量
						Even_howmany_package_Pre_process<<=8;
						Even_howmany_package_Pre_process|=Modbus_0x10_Value[1];
						image_Even_howmany_package_Pre_process=Even_howmany_package_Pre_process;//初始化工作
						STMFLASH_Write(FF_image_Even_howmany_package_Pre_process,(u16*)&image_Even_howmany_package_Pre_process,4);//参数多少袋平均修正一次提前量
						STMFLASH_Write(FF_Even_howmany_package_Pre_process,(u16*)&Even_howmany_package_Pre_process,4);
					}

//int Delay_slow_feed;//参数慢加延时
					if(Modbus_Address==Delay_slow_feed_address)//
					{
						Delay_slow_feed=Modbus_0x10_Value[0];//参数慢加延时
						Delay_slow_feed<<=8;
						Delay_slow_feed|=Modbus_0x10_Value[1];
					   STMFLASH_Write(FF_Delay_slow_feed,(u16*)&Delay_slow_feed,4);
					}

//int Delay_slow_feed;//参数初加延时
					if(Modbus_Address==Delay_initial_feed_address)//
					{
						Delay_initial_feed=Modbus_0x10_Value[0];//参数初加延时
						Delay_initial_feed<<=8;
						Delay_initial_feed|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Delay_initial_feed,(u16*)&Delay_initial_feed,4);
					}

//4.控制数据组 
//int Slow_feed_modify_uper_limt;//参数慢加修正上限
					if(Modbus_Address==Slow_feed_modify_uper_limt_address)//
					{
						Slow_feed_modify_uper_limt=Modbus_0x10_Value[0];//参数慢加修正上限
						Slow_feed_modify_uper_limt<<=8;
						Slow_feed_modify_uper_limt|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Slow_feed_modify_uper_limt,(u16*)&Slow_feed_modify_uper_limt,4);
					}
//int Slow_feed_modify_down_limt;//参数慢加修正下限
					if(Modbus_Address==Slow_feed_modify_down_limt_address)//	  
					{
						Slow_feed_modify_down_limt=Modbus_0x10_Value[0];//参数慢加修正下限
						Slow_feed_modify_down_limt<<=8;
						Slow_feed_modify_down_limt|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Slow_feed_modify_down_limt,(u16*)&Slow_feed_modify_down_limt,4);
					}
//int Pre_feed_modify_uper_limt;//参数提前量修正上限
					if(Modbus_Address==Pre_feed_modify_uper_limt_address)//
					{
						Pre_feed_modify_uper_limt=Modbus_0x10_Value[0];//参数提前量修正上限
						Pre_feed_modify_uper_limt<<=8;
						Pre_feed_modify_uper_limt|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Pre_feed_modify_uper_limt,(u16*)&Pre_feed_modify_uper_limt,4);
					}
//int Pre_feed_modify_down_limt;//参数提前量修正下限
					if(Modbus_Address==Pre_feed_modify_down_limt_address)//
					{
						Pre_feed_modify_down_limt=Modbus_0x10_Value[0];//参数提前量修正下限
						Pre_feed_modify_down_limt<<=8;
						Pre_feed_modify_down_limt|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Pre_feed_modify_down_limt,(u16*)&Pre_feed_modify_down_limt,4);
					}

//int Slow_feed;//参数慢加量
					if(Modbus_Address==Slow_feed_address)//
					{
						Slow_feed=Modbus_0x10_Value[0];//参数慢加量
						Slow_feed<<=8;
						Slow_feed|=Modbus_0x10_Value[1];

						if(Slow_feed>=Slow_feed_modify_down_limt&&Slow_feed<=Slow_feed_modify_uper_limt)  //*2017-10-28am*
							{	//image_Pre_feed=Pre_feed;//初始化工作
								STMFLASH_Write(FF_Slow_feed,(u16*)&Slow_feed,4);
								err_setting_Slow_feed=0;
//								Signal_lamp[4]=0x00; //慢加上下限出正确灭亮面板灯 
//								STMFLASH_Write(FF_image_Pre_feed,(u16*)&image_Pre_feed,4);//参数提前量
							} //*2017-10-28am*
						else
								{STMFLASH_Read(FF_Slow_feed,(u16*)&Slow_feed,4);
								err_setting_Slow_feed=1;
								}
//								Signal_lamp[4]=0x02; //慢加上下限出错点亮面板灯 
//								 Signal_lamp[5]=0x00; 
//				Signal_lamp[6]=0x00;   Signal_lamp[7]=0x00;
					}
//int Pre_feed;//参数提前量		 
					if(Modbus_Address==Pre_feed_address)//
					{
						Pre_feed=Modbus_0x10_Value[0];//参数提前量
						Pre_feed<<=8;
						Pre_feed|=Modbus_0x10_Value[1];
						if(Pre_feed>=Pre_feed_modify_down_limt&&Pre_feed<=Pre_feed_modify_uper_limt)  //*2017-10-28am*
							{	image_Pre_feed=Pre_feed;//初始化工作
								STMFLASH_Write(FF_Pre_feed,(u16*)&Pre_feed,4);
								STMFLASH_Write(FF_image_Pre_feed,(u16*)&image_Pre_feed,4);//参数提前量
								err_setting_Pre_feed=0;
							} //*2017-10-28am*
						else
						     {STMFLASH_Read (FF_Pre_feed,(u16*)&Pre_feed,4);
							  err_setting_Pre_feed=1;
							 }//*2017-10-28am*							
					}
//int Mid_feed;//参数中加值
					if(Modbus_Address==Mid_feed_address)//
					{
						Mid_feed=Modbus_0x10_Value[0];//参数中加值
						Mid_feed<<=8;
						Mid_feed|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Mid_feed,(u16*)&Mid_feed,4);
					}
//int Dlet_error;//参数允差
 					if(Modbus_Address==Dlet_error_address)//
					{
						Dlet_error=Modbus_0x10_Value[0];//参数允差
						Dlet_error<<=8;
						Dlet_error|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Dlet_error,(u16*)&Dlet_error,4);
					}
//int Target;//参数定值
					if(Modbus_Address==Target_address)//
					{
						Target=Modbus_0x10_Value[0];//参数定值
						Target<<=8;
						Target|=Modbus_0x10_Value[1];
						if(Target>=Target_down_limt&&Target<=Target_uper_limt)  //*2017-10-28am*
							{STMFLASH_Write(FF_Target,(u16*)&Target,4);
							err_setting_Target=0;
							} //*2017-10-28am*
						else
						     {STMFLASH_Read (FF_Target,(u16*)&Target,4);
							   err_setting_Target=1;
							 
							 }//*2017-10-28am*							
					}
//int Target_uper_limt;//参数定量上限
					if(Modbus_Address==Target_uper_limt_address)//
					{
						Target_uper_limt=Modbus_0x10_Value[0];//参数定量上限
						Target_uper_limt<<=8;
						Target_uper_limt|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Target_uper_limt,(u16*)&Target_uper_limt,4);
					}
//int Target_down_limt;//参数定量下限
 					if(Modbus_Address==Target_down_limt_address)//
					{
						Target_down_limt=Modbus_0x10_Value[0];//参数定量下限
						Target_down_limt<<=8;
						Target_down_limt|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Target_down_limt,(u16*)&Target_down_limt,4);
					}
//int zero_range;//参数零区
 					if(Modbus_Address==zero_range_address)//
					{
						zero_range=Modbus_0x10_Value[0];//参数零区
						zero_range<<=8;
						zero_range|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_zero_range,(u16*)&zero_range,4);
					}
//int Slow_feed_modify;//参数慢加修正量
					if(Modbus_Address==Slow_feed_modify_address)//
					{
						Slow_feed_modify=Modbus_0x10_Value[0];//参数慢加修正量
						Slow_feed_modify<<=8;
						Slow_feed_modify|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Slow_feed_modify,(u16*)&Slow_feed_modify,4);
					}

//int Initial_power_value;//参数初上电值
					if(Modbus_Address==Initial_power_value_address)//
					{
						Initial_power_value=Modbus_0x10_Value[0];//参数初上电值
						Initial_power_value<<=8;
						Initial_power_value|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Initial_power_value,(u16*)&Initial_power_value,4);
					}
//int Initial_feed_value;//参数初加量
					if(Modbus_Address==Initial_feed_value_address)//
					{
						Initial_feed_value=Modbus_0x10_Value[0];//参数初加量
						Initial_feed_value<<=8;
						Initial_feed_value|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Initial_feed_value,(u16*)&Initial_feed_value,4);
					}
//int Random_setting_value;//参数随机设定值
					if(Modbus_Address==Random_setting_value_address)//
					{
						Random_setting_value=Modbus_0x10_Value[0];//参数随机设定值
						Random_setting_value<<=8;
						Random_setting_value|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Random_setting_value,(u16*)&Random_setting_value,4);
					}

//5.修正数据组
//int ID_user_cal ;//参数用户标定关联号
					if(Modbus_Address==ID_user_cal_address)//
					{
						ID_user_cal=Modbus_0x10_Value[0];//参数用户标定关联号
						ID_user_cal<<=8;
						ID_user_cal|=Modbus_0x10_Value[1];
						set_shut_initial_mid_slow_prefeed_timer=ID_user_cal;  //*2017-10-28am*	set_shut_initial_mid_slow_prefeed_timer  借用	   ID_user_cal
						STMFLASH_Write(FF_ID_user_cal,(u16*)&ID_user_cal,4);
					}
//int ID_slow_feed_modify ;//参数慢加修正标记关联号
					if(Modbus_Address==ID_slow_feed_modify_address)//
					{
						ID_slow_feed_modify=Modbus_0x10_Value[0];//参数慢加修正标记关联号
						ID_slow_feed_modify<<=8;
						ID_slow_feed_modify|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_ID_slow_feed_modify,(u16*)&ID_slow_feed_modify,4);
					}
//int ID_pre_feed_modify;//参数提前修正标记关联号
					if(Modbus_Address==ID_pre_feed_modify_address)//
					{
						ID_pre_feed_modify=Modbus_0x10_Value[0];//参数提前修正标记关联号
						ID_pre_feed_modify<<=8;
						ID_pre_feed_modify|=Modbus_0x10_Value[1];
 						STMFLASH_Write(FF_ID_pre_feed_modify,(u16*)&ID_pre_feed_modify,4);
					}
//int ID_run_mode;//参数运行模式关联号
					if(Modbus_Address==ID_run_mode_address)//
					{
						ID_run_mode=Modbus_0x10_Value[0];//参数运行模式关联号
						ID_run_mode<<=8;
						ID_run_mode|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_ID_run_mode,(u16*)&ID_run_mode,4);
					}


//7.标定数据组
//int Cal_ID_division;//标定分度数关联号
					if(Modbus_Address==Cal_ID_division_address)//
					{
						Cal_ID_division=Modbus_0x10_Value[0];//标定分度数关联号
						Cal_ID_division<<=8;
						Cal_ID_division|=Modbus_0x10_Value[1];
  						STMFLASH_Write(FF_Cal_ID_division,(u16*)&Cal_ID_division,4);
					}


//int Cal_full_Mig;//标定用户砝码重量
					if(Modbus_Address==Cal_user_weight_address)//标定用户砝码重量
					{
						Modbus_Cal_user_weight.user_weight[3]=0;//Modbus_0x10_Value[0];
						Modbus_Cal_user_weight.user_weight[2]=0;//Modbus_0x10_Value[1];
						Modbus_Cal_user_weight.user_weight[1]=Modbus_0x10_Value[0];
						Modbus_Cal_user_weight.user_weight[0]=Modbus_0x10_Value[1];
						Cal_user_weight=(float)Modbus_Cal_user_weight.Cal_user_weight;
						STMFLASH_Write(FF_Cal_user_weight,(u16*)&Cal_user_weight,4);
					}
//int Cal_user_weight;//标定砝码重量
					if(Modbus_Address==Cal_weight_address)//标定砝码重量
					{
						Modbus_Cal_weight.add_weight[3]=0;//Modbus_0x10_Value[0];
						Modbus_Cal_weight.add_weight[2]=0;//Modbus_0x10_Value[1];
						Modbus_Cal_weight.add_weight[1]=Modbus_0x10_Value[0];
						Modbus_Cal_weight.add_weight[0]=Modbus_0x10_Value[1];
						Cal_weight=(float)Modbus_Cal_weight.Cal_weight;
  						STMFLASH_Write(FF_Cal_weight,(u16*)&Cal_weight,4);
					}
//8.滤波数据组：
//int Range_Filter0;//参数滤波范围
					if(Modbus_Address==Range_Filter0_address)//参数滤波范围
					{
						Range_Filter0=Modbus_0x10_Value[0];
						Range_Filter0<<=8;
						Range_Filter0|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Range_Filter0,(u16*)&Range_Filter0,5);
					}
 //int Range_Filter1;//参数滤波范围
					if(Modbus_Address==Range_Filter1_address)//参数滤波范围
					{
						Range_Filter1=Modbus_0x10_Value[0];
						Range_Filter1<<=8;
						Range_Filter1|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Range_Filter1,(u16*)&Range_Filter1,5);
					}
 //int Range_Filter2;//参数滤波范围
					if(Modbus_Address==Range_Filter2_address)//参数滤波范围
					{
						Range_Filter2=Modbus_0x10_Value[0];
						Range_Filter2<<=8;
						Range_Filter2|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Range_Filter2,(u16*)&Range_Filter2,5);
					}
 //int Range_Filter3;//参数滤波范围
					if(Modbus_Address==Range_Filter3_address)//参数滤波范围
					{
						Range_Filter3=Modbus_0x10_Value[0];
						Range_Filter3<<=8;
						Range_Filter3|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Range_Filter3,(u16*)&Range_Filter3,5);
					}
 //int Range_Filter4;//参数滤波范围
					if(Modbus_Address==Range_Filter4_address)//参数滤波范围
					{
						Range_Filter4=Modbus_0x10_Value[0];
						Range_Filter4<<=8;
						Range_Filter4|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Range_Filter4,(u16*)&Range_Filter4,5);
					}
					/*******************************/
//int Strong_Filter0;//参数滤波强度
					if(Modbus_Address==Strong_Filter0_address)//参数滤波强度
					{
						Strong_Filter0=Modbus_0x10_Value[0];
						Strong_Filter0<<=8;
						Strong_Filter0|=Modbus_0x10_Value[1]; //if(Strong_Filter0>31) Strong_Filter0=30;
						STMFLASH_Write(FF_Strong_Filter0,(u16*)&Strong_Filter0,4);
					}
 //int Strong_Filter1;//参数滤波强度
					if(Modbus_Address==Strong_Filter1_address)//参数滤波强度
					{
						Strong_Filter1=Modbus_0x10_Value[0];
						Strong_Filter1<<=8;
						Strong_Filter1|=Modbus_0x10_Value[1]; //if(Strong_Filter1>31) Strong_Filter1=30;
						STMFLASH_Write(FF_Strong_Filter1,(u16*)&Strong_Filter1,4);
					}
 //int Strong_Filter2;//参数滤波强度
					if(Modbus_Address==Strong_Filter2_address)//参数滤波强度
					{
						Strong_Filter2=Modbus_0x10_Value[0];
						Strong_Filter2<<=8;
						Strong_Filter2|=Modbus_0x10_Value[1]; //if(Strong_Filter2>31) Strong_Filter2=30;
						STMFLASH_Write(FF_Strong_Filter2,(u16*)&Strong_Filter2,4);
					}
 //int Strong_Filter3;//参数滤波强度
					if(Modbus_Address==Strong_Filter3_address)//参数滤波强度
					{
						Strong_Filter3=Modbus_0x10_Value[0];
						Strong_Filter3<<=8;
						Strong_Filter3|=Modbus_0x10_Value[1];//if(Strong_Filter3>31) Strong_Filter3=30;
						STMFLASH_Write(FF_Strong_Filter3,(u16*)&Strong_Filter3,4);
					}
 //int Strong_Filter4;//参数滤波强度
					if(Modbus_Address==Strong_Filter4_address)//参数滤波强度
					{
						Strong_Filter4=Modbus_0x10_Value[0];
						Strong_Filter4<<=8;
						Strong_Filter4|=Modbus_0x10_Value[1];//if(Strong_Filter4>31) Strong_Filter4=30;
						STMFLASH_Write(FF_Strong_Filter4,(u16*)&Strong_Filter4,4);
					}

					  
 //int Cal_full_Mig;//满量程值
					if(Modbus_Address==Cal_full_Mig_address)//满量程值
					{
						Cal_full_Mig=Modbus_0x10_Value[0];
						Cal_full_Mig<<=8;
						Cal_full_Mig|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Cal_full_Mig,(u16*)&Cal_full_Mig,4);

//						waitting_for_KLTT_PARA=Cal_full_Mig;
						
					}
//int Cal_point;//标定小数点
					if(Modbus_Address==Cal_pint_address)//标定小数点
					{
						Cal_point=Modbus_0x10_Value[0];
						Cal_point<<=8;
						Cal_point|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Cal_point,(u16*)&Cal_point,4);
					}

//Analog_mode_SEL_address 模拟量输出选择
					if(Modbus_Address==Analog_mode_SEL_address)//模拟量输出选择
					{
						Analog_mode_SEL=Modbus_0x10_Value[0];
						Analog_mode_SEL<<=8;
						Analog_mode_SEL|=Modbus_0x10_Value[1];
						STMFLASH_Write(FF_Analog_mode_SEL,(u16*)&Analog_mode_SEL,4);
					} 


//8.滤波数据组：
//#define Range_Filter0_address Cal_weight_address+2    //参数滤波范围0
//#define Range_Filter1_address Range_Filter0_address+1//参数滤波范围1
//#define Range_Filter2_address Range_Filter1_address+1//参数滤波范围2
//#define Range_Filter3_address Range_Filter2_address+1//参数滤波范围3
//#define Range_Filter4_address Range_Filter3_address+1//参数滤波范围4
//
//#define Strong _Filter0_address Range_Filter4_address+1//参数滤波强度0
//#define Strong_Filter1_address Strong_Filter0_address+1//参数滤波强度1
//#define Strong_Filter2_address Strong_Filter1_address+1//参数滤波强度2
//#define Strong_Filter3_address Strong_Filter2_address+1//参数滤波强度3
//#define Strong_Filter4_address Strong_Filter3_address+1//参数滤波强度4


			 break;	 
		     case 10:
					Modbus_CRC_CODE=Data;
					Modbus_CRC_CODE<<=8;
					Modbus_Counter++;
			  break;
		     case 11:
			        Modbus_CRC_CODE|=Data;

								len=7+Modbus_value_count;
								        p=Modbus_Data;
										*(p++)=0xc8;
										*(p++)=Modbus_function;
										*(p++)=Modbus_Address>>8;
										*(p++)=Modbus_Address&0x000000ff;
										*(p++)=Modbus_value>>8;
										*(p++)=Modbus_value&0x000000ff;
									    *(p++)=Modbus_byte_count;
										for(i=0;i<Modbus_byte_count;i++)
											{*(p++)=Modbus_0x10_Value[i];}
 								px=mtb_crc_calc((unsigned char *)Modbus_Data,len);
								temps[0]=px&0x0ff;					             
								temps[1]=(px>>8)&0x0ff;	
								return_crc=temps[0];
								return_crc<<=8;
								return_crc|=temps[1];
								p=Modbus_Data;
								if(return_crc==Modbus_CRC_CODE)
									{
										*(p++)=0xc8;
										*(p++)=Modbus_function;
										*(p++)=Modbus_Address>>8;
										*(p++)=Modbus_Address&0x000000ff;
										*(p++)=Modbus_value>>8;
										*(p++)=Modbus_value&0x000000ff;
										len=6;
										px=mtb_crc_calc((unsigned char *)Modbus_Data,len);
										temps[0]=px&0x0ff;					             
										temps[1]=(px>>8)&0x0ff;	
										*(p++)=temps[0];	
										*(p++)=temps[1];
										*(p++)=0;//end
										p=Modbus_Data;
										len+=2;
										for(i=0;i<len;i++)
										{
											USART_SendData(USART1,*(p++));
											while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
										}
									} //ok
							    else
								    {
										*(p++)=0xc8;
										*(p++)=0x90;
										*(p++)=0x01;
									}//error
					Modbus_Data[0]=0;
					Modbus_function=0;//if CRC ok 
					Modbus_Counter=0;
			  break;
			default:
						   Modbus_Data[0]=0;
					       Modbus_function=0;//if CRC ok 
						   Modbus_Counter=0;
				break;
		}
	}
//}






/**************************************************************************/
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;
    //使能外部晶振
    RCC_HSEConfig(RCC_HSE_ON);
    //等待外部晶振稳定
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    //如果外部晶振启动成功，则进行下一步操作
    if(HSEStartUpStatus==SUCCESS)
    {
        //设置HCLK（AHB时钟）=SYSCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //PCLK1(APB1) = HCLK/2
        RCC_PCLK1Config(RCC_HCLK_Div2);
        //PCLK2(APB2) = HCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);
        //FLASH时序控制
        //推荐值：SYSCLK = 0~24MHz   Latency=0
        //        SYSCLK = 24~48MHz  Latency=1
        //        SYSCLK = 48~72MHz  Latency=2
        FLASH_SetLatency(FLASH_Latency_2);
        //开启FLASH预取指功能
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        //PLL设置 SYSCLK/1 * 9 = 8*1*9 = 72MHz
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        //启动PLL
        RCC_PLLCmd(ENABLE);
        //等待PLL稳定
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        //系统时钟SYSCLK来自PLL输出
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        //切换时钟后等待系统时钟稳定
        while(RCC_GetSYSCLKSource()!=0x08);

        /*
        //设置系统SYSCLK时钟为HSE输入
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
        //等待时钟切换成功
        while(RCC_GetSYSCLKSource() != 0x04);
        */
    }
    //下面是给各模块开启时钟
    //启动GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | \
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,\
                           ENABLE);
    //启动AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //启动USART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //启动DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
}

void DMA_Configuration(char *SendBuff,u16 SENDBUFF_SIZE)
{
    DMA_InitTypeDef DMA_InitStructure;
    //DMA设置：
    //设置DMA源：内存地址&串口数据寄存器地址
    //方向：内存-->外设
    //每次传输位：8bit
    //传输大小DMA_BufferSize=SENDBUFF_SIZE
    //地址自增模式：外设地址不增，内存地址自增1
    //DMA模式：一次传输，非循环
    //优先级：中
    DMA_DeInit(DMA1_Channel4);//串口1的DMA传输通道是通道4
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设作为DMA的目的端
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;//传输大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不增加
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址自增1
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//(DMA传送优先级为中等)
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
}

void DMA_Configuration_for_recive(char *SendBuff,u16 SENDBUFF_SIZE)
{
    DMA_InitTypeDef DMA_InitStructure;
    //DMA设置：
    //设置DMA源：内存地址&串口数据寄存器地址
    //方向：内存-->外设
    //每次传输位：8bit
    //传输大小DMA_BufferSize=SENDBUFF_SIZE
    //地址自增模式：外设地址不增，内存地址自增1
    //DMA模式：一次传输，非循环
    //优先级：中
    DMA_DeInit(DMA1_Channel5);//串口1的DMA传输通道是通道5
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;
    DMA_InitStructure.DMA_DIR =  DMA_DIR_PeripheralSRC;//外设作为DMA的源端
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;//传输大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不增加
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址自增1
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//(DMA传送优先级为中等)
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
}


void TestY_output(int Modbus_Address )
{
		if (Modbus_Address==Test_Y00_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_00;}
		   else
			  {Y_output_lowx_00;}
		 }

		if (Modbus_Address==Test_Y01_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_01;}
		   else
			  {Y_output_lowx_01;}
		 }

		if (Modbus_Address==Test_Y02_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_02;}
		   else
			  {Y_output_lowx_02;}
		 }

		if (Modbus_Address==Test_Y03_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_03;}
		   else
			  {Y_output_lowx_03;}
		 }

		if (Modbus_Address==Test_Y04_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_04;}
		   else
			  {Y_output_lowx_04;}
		 }

		if (Modbus_Address==Test_Y05_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_05;}
		   else
			  {Y_output_lowx_05;}
		 }

		if (Modbus_Address==Test_Y06_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_06;}
		   else
			  {Y_output_lowx_06;}
		 }

		if (Modbus_Address==Test_Y07_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_07;}
		   else
			  {Y_output_lowx_07;}
		 }

		if (Modbus_Address==Test_Y08_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_08;}
		   else
			  {Y_output_lowx_08;}
		 }

		if (Modbus_Address==Test_Y09_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_09;}
		   else
			  {Y_output_lowx_09;}
		 }


		if (Modbus_Address==Test_Y10_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_0A;}
		   else
			  {Y_output_lowx_0A;}
		 }

		if (Modbus_Address==Test_Y11_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_0B;}
		   else
			  {Y_output_lowx_0B;}
		 }

		if (Modbus_Address==Test_Y12_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_0C;}
		   else
			  {Y_output_lowx_0C;}
		 }

		if (Modbus_Address==Test_Y13_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_0D;}
		   else
			  {Y_output_lowx_0D;}
		 }

		if (Modbus_Address==Test_Y14_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_0E;}
		   else
			  {Y_output_lowx_0E;}
		 }

		if (Modbus_Address==Test_Y15_pressed)
		 {
		   if(Modbus_value==0xff00)//open
			  {Y_output_high_0F;}
		   else
			  {Y_output_lowx_0F;}
		 }

}


void USART2_Process(void)
{
char x00,Data;
// if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
// {x00+=9;}
// 	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);


	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Data = (unsigned char)USART_ReceiveData(USART2);

		recive_AD_code(Data); supervision_ad_com=0;
		GPIO_WriteBit(GPIOE, GPIO_Pin_6, (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_6))));//第一LED:通讯
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
			USART_ClearFlag(USART2, USART_IT_RXNE);
	}

//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		Data = (unsigned char)USART_ReceiveData(USART2);
//		if(	Data ==0xab)
//		 {
//		    USART2_RE_high;
//			USART2_DE_high;
//			USART_SendData(USART2,0xcd);
//			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//			USART_SendData(USART2,0x12);
//			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//			USART_SendData(USART2,' ');
//			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); //不发送
////			elimate_USART_DE[2]=3;
//			USART2_RE_lowx;
//			USART2_DE_lowx;
//		 }
//	}

}
void USART3_Process(void)
{
//char Data;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
			Datax = (unsigned char)USART_ReceiveData(USART3);
			PC_comm3_command(Datax);

//			if(	Datax ==Comm_Addr+'B')
//			{
//				 Pc_recived=1;
//			}
//			USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//			USART_ClearFlag(USART3, USART_IT_RXNE);
	}
}
void UART4_Process(void)
{
char x00,Data;
 if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
 {x00+=9;}
 	 USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		Data = (unsigned char)USART_ReceiveData(UART4);
		if(	Data ==0xab)
		 {
		    USART2_RE_high;
			USART2_DE_high;
			USART_SendData(UART4,0x56);
			while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
			elimate_USART_DE[4]=4;
			USART2_RE_lowx;
//			USART2_DE_lowx;
		 }
	}

}
void UART5_Process(void)
{
char x00,Data;
 if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
 {x00+=9;}
 	 USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		Data = (unsigned char)USART_ReceiveData(UART5);
		if(	Data ==0xab)
		 {
		    USART2_RE_high;
			USART2_DE_high;
			USART_SendData(UART5,0x45);
			while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
			elimate_USART_DE[5]=4;
			USART2_RE_lowx;
//			USART2_DE_lowx;
		 }
	}

}

void Recover_para(void)
{
//1.通讯数据组
STMFLASH_Read (FF_ID_budrate,(u16*)&ID_budrate,4);
STMFLASH_Read (FF_Comm_Addr,(u16*)&Comm_Addr,4);
//2.置零数据组
STMFLASH_Read (FF_Initial_zero_range,(u16*)&Initial_zero_range,4);
STMFLASH_Read (FF_Munal_zero_range,(u16*)&Munal_zero_range,4);
STMFLASH_Read (FF_Trace_zero_range,(u16*)&Trace_zero_range,4);
//3.延时数据组
STMFLASH_Read (FF_Delay_tare,(u16*)&Delay_tare,4);
STMFLASH_Read (FF_Delay_Mid_feed,(u16*)&Delay_Mid_feed,4);
STMFLASH_Read (FF_Delay_Pre_process,(u16*)&Delay_Pre_process,4);
STMFLASH_Read (FF_Delay_Next_loop,(u16*)&Delay_Next_loop,4);
STMFLASH_Read (FF_Time_supvisor_Slow_feed_up_limt,(u16*)&Time_supvisor_Slow_feed_up_limt,4);
STMFLASH_Read (FF_Time_supvisor_Slow_feed_low_limt,(u16*)&Time_supvisor_Slow_feed_low_limt,4);

STMFLASH_Read (FF_Pulse_howmany_package,(u16*)&Pulse_howmany_package,4);
STMFLASH_Read (FF_Tare_howmany_package,(u16*)&Tare_howmany_package,4);
STMFLASH_Read (FF_Recive_x_signals,(u16*)&Recive_x_signals,4);
STMFLASH_Read (FF_no_recive_x_signals,(u16*)&no_recive_x_signals,4);
STMFLASH_Read (FF_Even_howmany_package_Pre_process,(u16*)&Even_howmany_package_Pre_process,4);
STMFLASH_Read (FF_Delay_slow_feed,(u16*)&Delay_slow_feed,4);
STMFLASH_Read (FF_Delay_initial_feed,(u16*)&Delay_initial_feed,4);
//4.控制数据组   
STMFLASH_Read (FF_Slow_feed_modify_uper_limt,(u16*)&Slow_feed_modify_uper_limt,4);
STMFLASH_Read (FF_Slow_feed_modify_down_limt,(u16*)&Slow_feed_modify_down_limt,4);
STMFLASH_Read (FF_Pre_feed_modify_uper_limt,(u16*)&Pre_feed_modify_uper_limt,4);
STMFLASH_Read (FF_Pre_feed_modify_down_limt,(u16*)&Pre_feed_modify_down_limt,4);
STMFLASH_Read (FF_Slow_feed,(u16*)&Slow_feed,4);
STMFLASH_Read (FF_Pre_feed,(u16*)&Pre_feed,4);
STMFLASH_Read (FF_Mid_feed,(u16*)&Mid_feed,4);
STMFLASH_Read (FF_Dlet_error,(u16*)&Dlet_error,4);
STMFLASH_Read (FF_Target,(u16*)&Target,4);
STMFLASH_Read (FF_Target_uper_limt,(u16*)&Target_uper_limt,4);
STMFLASH_Read (FF_Target_down_limt,(u16*)&Target_down_limt,4);
STMFLASH_Read (FF_zero_range,(u16*)&zero_range,4);
STMFLASH_Read (FF_Slow_feed_modify,(u16*)&Slow_feed_modify,4);
STMFLASH_Read (FF_Initial_power_value,(u16*)&Initial_power_value,4);
STMFLASH_Read (FF_Initial_feed_value,(u16*)&Initial_feed_value,4);
STMFLASH_Read (FF_Random_setting_value,(u16*)&Random_setting_value,4);
//5.修正数据组
STMFLASH_Read (FF_ID_user_cal,(u16*)&ID_user_cal,4);
STMFLASH_Read (FF_ID_slow_feed_modify,(u16*)&ID_slow_feed_modify,4);
STMFLASH_Read (FF_ID_pre_feed_modify,(u16*)&ID_pre_feed_modify,4);
STMFLASH_Read (FF_ID_run_mode,(u16*)&ID_run_mode,4);
//7.标定数据组
STMFLASH_Read (FF_Cal_ID_division,(u16*)&Cal_ID_division,4);
STMFLASH_Read (FF_Cal_user_weight,(u16*)&Cal_user_weight,4);
STMFLASH_Read (FF_Cal_weight,(u16*)&Cal_weight,4);
//8.滤波数据组：
STMFLASH_Read (FF_Range_Filter0,(u16*)&Range_Filter0,5);
STMFLASH_Read (FF_Range_Filter1,(u16*)&Range_Filter1,5);
STMFLASH_Read (FF_Range_Filter2,(u16*)&Range_Filter2,5);
STMFLASH_Read (FF_Range_Filter3,(u16*)&Range_Filter3,5);
STMFLASH_Read (FF_Range_Filter4,(u16*)&Range_Filter4,5);

STMFLASH_Read (FF_Strong_Filter0,(u16*)&Strong_Filter0,4);
STMFLASH_Read (FF_Strong_Filter1,(u16*)&Strong_Filter1,4);
STMFLASH_Read (FF_Strong_Filter2,(u16*)&Strong_Filter2,4);
STMFLASH_Read (FF_Strong_Filter3,(u16*)&Strong_Filter3,4);
STMFLASH_Read (FF_Strong_Filter4,(u16*)&Strong_Filter4,4);

STMFLASH_Read (FF_Cal_full_Mig,(u16*)&Cal_full_Mig,4);
STMFLASH_Read (FF_Cal_point,(u16*)&Cal_point,4);

STMFLASH_Read (FF_Target_pulse,(u16*)&Modbus_pulse.target_totall_pulse,4);

STMFLASH_Read (FF_image_Pulse_howmany_package,(u16*)&image_Pulse_howmany_package,4);//参数多少袋输出脉冲
STMFLASH_Read (FF_image_Tare_howmany_package,(u16*)&image_Tare_howmany_package,4);//参数多少袋去皮一次
STMFLASH_Read (FF_image_Even_howmany_package_Pre_process,(u16*)&image_Even_howmany_package_Pre_process,4);//参数多少袋平均修正一次提前量
STMFLASH_Read (FF_image_Pre_feed,(u16*)&image_Pre_feed,2);//参数提前量
//STMFLASH_Read (FF_,(u16*)&,4);

STMFLASH_Read (FF_Analog_mode_SEL,(u16*)&Analog_mode_SEL,4); //模拟量输出选择

}

void Write_image_para(char i)
{
//#define XX_image_Pulse_howmany_package 1
//#define XX_image_Tare_howmany_package  2
//#define XX_image_Even_howmany_package_Pre_process  3
//#define XX_image_Pre_feed  4 


	switch (i)
	{
		case 1:
				STMFLASH_Write (FF_image_Pulse_howmany_package,(u16*)&image_Pulse_howmany_package,4);//参数多少袋输出脉冲
			break;
		case 2:
				STMFLASH_Write (FF_image_Tare_howmany_package,(u16*)&image_Tare_howmany_package,4);//参数多少袋去皮一次
			break;
		case 3:
				STMFLASH_Write (FF_image_Even_howmany_package_Pre_process,(u16*)&image_Even_howmany_package_Pre_process,4);//参数多少袋平均修正一次提前量
			break;
		case 4:
				STMFLASH_Write (FF_image_Pre_feed,(u16*)&image_Pre_feed,4);//参数提前量
			break;	
		default:
			break;
	}

}

void Reply_pc_inquier(void)
{
char i;
if(Pc_recived)
{
	Pc_recived=0;
	status_and_datum();
	USART3_RE_high;
	USART3_DE_high;
	 for(i=0;i<6;i++)
		{	USART_SendData(USART3,status[i]);
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		}
			USART_SendData(USART3,' ');
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	
			USART3_RE_lowx;
			USART3_DE_lowx;

}
}

void return_data(unsigned char *comm3_reply_point)
{
	USART3_RE_high;
	USART3_DE_high;
	while(*comm3_reply_point)
		{	USART_SendData(USART3,*comm3_reply_point); comm3_reply_point++;
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		}
 			USART3_RE_lowx;
			USART3_DE_lowx;

}

void return_data_xx(unsigned char *comm3_reply_point)
{
	USART3_RE_high;
	USART3_DE_high;
			 USART_SendData(USART3,'Y'); 
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	while(*comm3_reply_point)
		{	USART_SendData(USART3,*comm3_reply_point); comm3_reply_point++;
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		}
			 USART_SendData(USART3,'X'); 
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
			 USART_SendData(USART3,'X'); 
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
 			USART3_RE_lowx;
			USART3_DE_lowx;

}





//要写入到STM32 FLASH的字符串数组
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)	 			  	//数组长度
#define FLASH_SAVE_ADDR  0X08070000 				//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define ADDR_POINTER FLASH_SAVE_ADDR+3*1024
#define Totall_Express ADDR_POINTER+200
#define Expression_Len Totall_Express+200
#define Weight_para Expression_Len+1024*2
#define Step_para Weight_para+1024


#define AD_ZEROX  Step_para +1024
#define AD_COFFX  AD_ZEROX +10

//#define FL_Weighting_T  AD_COFFX +10
//#define FL_Weighting_package  FL_Weighting_T +10

#define FL_Weighting_package  AD_COFFX +10
#define FL_Weighting_T  FL_Weighting_package +10

// //1.通讯数据组
//#define  FF_ID_budrate  FL_Weighting_package+4//参数波特率关联号
//#define  FF_Comm_Addr   FF_ID_budrate+4//参数通讯地址关联号
////2.置零数据组
//#define  FF_Initial_zero_range  FF_Comm_Addr+4//参数开机置零范围
//#define  FF_Munal_zero_range    FF_Initial_zero_range+4//参数手动置零范围
//#define  FF_Trace_zero_range    FF_Munal_zero_range+4//参数零跟踪范围
////3.延时数据组
//#define  FF_Delay_tare                        FF_Trace_zero_range+4//参数去皮延时输出
//#define  FF_Delay_Mid_feed                    FF_Delay_tare+4//参数中加延时
//#define  FF_Delay_Pre_process                 FF_Delay_Mid_feed+4//参数提前延时
//#define  FF_Delay_Next_loop                   FF_Delay_Pre_process+4//参数下一循环延时
//#define  FF_Time_supvisor_Slow_feed_up_limt   FF_Delay_Next_loop+4//参数慢加监视上限时间
//#define  FF_Time_supvisor_Slow_feed_low_limt  FF_Time_supvisor_Slow_feed_up_limt+4//参数慢加监视下限时间
//#define  FF_Pulse_howmany_package             FF_Time_supvisor_Slow_feed_low_limt+4//参数多少袋输出脉冲
//#define  FF_Tare_howmany_package              FF_Pulse_howmany_package+4//参数多少袋去皮一次
//#define  FF_Recive_x_signals                  FF_Tare_howmany_package+4//参数收到x次信号设定
//#define  FF_no_recive_x_signals               FF_Recive_x_signals+4//参数收不到x次信号设定限
//#define  FF_Even_howmany_package_Pre_process  FF_no_recive_x_signals+4//参数多少袋平均修正一次提前量
//#define  FF_Delay_slow_feed                   FF_Even_howmany_package_Pre_process+4//参数慢加延时
//#define  FF_Delay_initial_feed                FF_Delay_slow_feed+4//参数初加延时
//
////4.控制数据组  FF_ 
//#define  FF_Slow_feed_modify_uper_limt  FF_Delay_initial_feed+4//参数慢加修正上限
//#define  FF_Slow_feed_modify_down_limt  FF_Slow_feed_modify_uper_limt+4//参数慢加修正下限
//#define  FF_Pre_feed_modify_uper_limt   FF_Slow_feed_modify_down_limt+4//参数提前量修正上限
//#define  FF_Pre_feed_modify_down_limt   FF_Pre_feed_modify_uper_limt+4//参数提前量修正下限
//#define  FF_Slow_feed                   FF_Pre_feed_modify_down_limt+4//参数慢加量
//#define  FF_Pre_feed                    FF_Slow_feed+4//参数提前量
//#define  FF_Mid_feed                    FF_Pre_feed+4//参数中加值
//#define  FF_Dlet_error                  FF_Mid_feed+4//参数允差
//#define  FF_Target                      FF_Dlet_error+4//参数定值
//#define  FF_Target_uper_limt            FF_Target+4//参数定量上限
//#define  FF_Target_down_limt            FF_Target_uper_limt+4//参数定量下限
//#define  FF_zero_range                  FF_Target_down_limt+4//参数零区
//#define  FF_Slow_feed_modify            FF_zero_range+4//参数慢加修正量
//#define  FF_Initial_power_value         FF_Slow_feed_modify+4//参数初上电值
//#define  FF_Initial_feed_value          FF_Initial_power_value+4//参数初加量
//#define  FF_Random_setting_value        FF_Initial_feed_value+4//参数随机设定值
//
////5.修正数据组
//#define  FF_ID_user_cal          FF_Random_setting_value+4//参数用户标定关联号
//#define  FF_ID_slow_feed_modify  FF_ ID_user_cal+4//参数慢加修正标记关联号
//#define  FF_ID_pre_feed_modify   FF_ID_slow_feed_modify+4//参数提前修正标记关联号
//#define  FF_ID_run_mode          FF_ID_pre_feed_modify+4//参数运行模式关联号
//
//
//
////6.杂项数据组
//#define  FF_Clear_merory_code  FF_ID_run_mode+4//内存清理密码
//#define  FF_Target_pulse       FF_Clear_merory_code+4//参数定量的总脉冲数
//#define  FF_ID_weight_unit     FF_Target_pulse+4//参数重量单位设定关联号
//#define  FF_Num_shift          FF_ID_weight_unit+4//参数班次
////7.#define  FF_ +4//标定数据组
//#define  FF_Cal_code         FF_Num_shift+4//标定密码
//#define  FF_Cal_point        FF_Cal_code+4//标定小数点
//#define  FF_Cal_ID_division  FF_Cal_point+4//标定分度数关联号
//#define  FF_Cal_full_Mig     FF_Cal_ID_division+4//标定满量程值
//#define  FF_Cal_user_weight  FF_Cal_full_Mig+4//标定用户砝码重量
////#define  FF_Cal_zero  FF_Cal_user_weight+4//标定零位
////#define  FF_Cal_ratio  FF_Cal_zero+4//标定标率
//#define  FF_Cal_weight       FF_Cal_user_weight+4	//标定砝码重量
//
////8.滤波数据组：
//#define  FF_Range_Filter0   FF_Cal_weight+4//参数滤波范围0
//#define  FF_Range_Filter1   FF_Range_Filter0+4//参数滤波范围1
//#define  FF_Range_Filter2   FF_Range_Filter1+4//参数滤波范围2
//#define  FF_Range_Filter3   FF_Range_Filter2+4//参数滤波范围3
//#define  FF_Range_Filter4   FF_Range_Filter3+4//参数滤波范围4
//#define  FF_Strong_Filter0  FF_Range_Filter4+4//参数滤波强度0
//#define  FF_Strong_Filter1  FF_Strong_Filter0+4//参数滤波强度1
//#define  FF_Strong_Filter2  FF_Strong_Filter1+4//参数滤波强度2
//#define  FF_Strong_Filter3  FF_Strong_Filter2+4//参数滤波强度3
//#define  FF_Strong_Filter4  FF_Strong_Filter3+4//参数滤波强度4
//////称重_吨位，袋数，平均值
////#define  FF_Weighting_T,Weighting_package,Weighting_even  FF_+4
//
//
//
//
//
//
//
//
//
//
//
//#define XXXXX09  FF_Strong_Filter4 +1024

u8 datatemp[SIZE];		 

void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)	 
{ 	   	 
	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器
	//用于标识向量表是在CODE区还是在RAM区
}

void MYRCC_DeInit(void)
{	
 	RCC->APB1RSTR = 0x00000000;//复位结束			 
	RCC->APB2RSTR = 0x00000000; 
	  
  	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
  	RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //关闭所有中断		 
	//配置向量表				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else   
	MY_NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
#endif
}
 void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;   
	MYRCC_DeInit();		  //复位并配置向量表
 	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	while(!(RCC->CR>>17));//等待外部时钟就绪
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;//抵消2个单位
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	FLASH->ACR|=0x32;	  //FLASH 2个延时周期

	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟	 
	while(temp!=0x02)     //等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	} 
}	
	   

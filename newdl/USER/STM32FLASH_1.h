
//Ҫд�뵽STM32 FLASH���ַ�������
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)	 			  	//���鳤��
#define FLASH_SAVE_ADDR  0X08070000 				//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
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

// //1.ͨѶ������
//#define  FF_ID_budrate  FL_Weighting_package+4//���������ʹ�����
//#define  FF_Comm_Addr   FF_ID_budrate+4//����ͨѶ��ַ������
////2.����������
//#define  FF_Initial_zero_range  FF_Comm_Addr+4//�����������㷶Χ
//#define  FF_Munal_zero_range    FF_Initial_zero_range+4//�����ֶ����㷶Χ
//#define  FF_Trace_zero_range    FF_Munal_zero_range+4//��������ٷ�Χ
////3.��ʱ������
//#define  FF_Delay_tare                        FF_Trace_zero_range+4//����ȥƤ��ʱ���
//#define  FF_Delay_Mid_feed                    FF_Delay_tare+4//�����м���ʱ
//#define  FF_Delay_Pre_process                 FF_Delay_Mid_feed+4//������ǰ��ʱ
//#define  FF_Delay_Next_loop                   FF_Delay_Pre_process+4//������һѭ����ʱ
//#define  FF_Time_supvisor_Slow_feed_up_limt   FF_Delay_Next_loop+4//�������Ӽ�������ʱ��
//#define  FF_Time_supvisor_Slow_feed_low_limt  FF_Time_supvisor_Slow_feed_up_limt+4//�������Ӽ�������ʱ��
//#define  FF_Pulse_howmany_package             FF_Time_supvisor_Slow_feed_low_limt+4//�������ٴ��������
//#define  FF_Tare_howmany_package              FF_Pulse_howmany_package+4//�������ٴ�ȥƤһ��
//#define  FF_Recive_x_signals                  FF_Tare_howmany_package+4//�����յ�x���ź��趨
//#define  FF_no_recive_x_signals               FF_Recive_x_signals+4//�����ղ���x���ź��趨��
//#define  FF_Even_howmany_package_Pre_process  FF_no_recive_x_signals+4//�������ٴ�ƽ������һ����ǰ��
//#define  FF_Delay_slow_feed                   FF_Even_howmany_package_Pre_process+4//����������ʱ
//#define  FF_Delay_initial_feed                FF_Delay_slow_feed+4//����������ʱ
//
////4.����������  FF_ 
//#define  FF_Slow_feed_modify_uper_limt  FF_Delay_initial_feed+4//����������������
//#define  FF_Slow_feed_modify_down_limt  FF_Slow_feed_modify_uper_limt+4//����������������
//#define  FF_Pre_feed_modify_uper_limt   FF_Slow_feed_modify_down_limt+4//������ǰ����������
//#define  FF_Pre_feed_modify_down_limt   FF_Pre_feed_modify_uper_limt+4//������ǰ����������
//#define  FF_Slow_feed                   FF_Pre_feed_modify_down_limt+4//����������
//#define  FF_Pre_feed                    FF_Slow_feed+4//������ǰ��
//#define  FF_Mid_feed                    FF_Pre_feed+4//�����м�ֵ
//#define  FF_Dlet_error                  FF_Mid_feed+4//�����ʲ�
//#define  FF_Target                      FF_Dlet_error+4//������ֵ
//#define  FF_Target_uper_limt            FF_Target+4//������������
//#define  FF_Target_down_limt            FF_Target_uper_limt+4//������������
//#define  FF_zero_range                  FF_Target_down_limt+4//��������
//#define  FF_Slow_feed_modify            FF_zero_range+4//��������������
//#define  FF_Initial_power_value         FF_Slow_feed_modify+4//�������ϵ�ֵ
//#define  FF_Initial_feed_value          FF_Initial_power_value+4//����������
//#define  FF_Random_setting_value        FF_Initial_feed_value+4//��������趨ֵ
//
////5.����������
//#define  FF_ID_user_cal          FF_Random_setting_value+4//�����û��궨������
//#define  FF_ID_slow_feed_modify  FF_ ID_user_cal+4//��������������ǹ�����
//#define  FF_ID_pre_feed_modify   FF_ID_slow_feed_modify+4//������ǰ������ǹ�����
//#define  FF_ID_run_mode          FF_ID_pre_feed_modify+4//��������ģʽ������
//
//
//
////6.����������
//#define  FF_Clear_merory_code  FF_ID_run_mode+4//�ڴ���������
//#define  FF_Target_pulse       FF_Clear_merory_code+4//������������������
//#define  FF_ID_weight_unit     FF_Target_pulse+4//����������λ�趨������
//#define  FF_Num_shift          FF_ID_weight_unit+4//�������
////7.#define  FF_ +4//�궨������
//#define  FF_Cal_code         FF_Num_shift+4//�궨����
//#define  FF_Cal_point        FF_Cal_code+4//�궨С����
//#define  FF_Cal_ID_division  FF_Cal_point+4//�궨�ֶ���������
//#define  FF_Cal_full_Mig     FF_Cal_ID_division+4//�궨������ֵ
//#define  FF_Cal_user_weight  FF_Cal_full_Mig+4//�궨�û���������
////#define  FF_Cal_zero  FF_Cal_user_weight+4//�궨��λ
////#define  FF_Cal_ratio  FF_Cal_zero+4//�궨����
//#define  FF_Cal_weight       FF_Cal_user_weight+4	//�궨��������
//
////8.�˲������飺
//#define  FF_Range_Filter0   FF_Cal_weight+4//�����˲���Χ0
//#define  FF_Range_Filter1   FF_Range_Filter0+4//�����˲���Χ1
//#define  FF_Range_Filter2   FF_Range_Filter1+4//�����˲���Χ2
//#define  FF_Range_Filter3   FF_Range_Filter2+4//�����˲���Χ3
//#define  FF_Range_Filter4   FF_Range_Filter3+4//�����˲���Χ4
//#define  FF_Strong_Filter0  FF_Range_Filter4+4//�����˲�ǿ��0
//#define  FF_Strong_Filter1  FF_Strong_Filter0+4//�����˲�ǿ��1
//#define  FF_Strong_Filter2  FF_Strong_Filter1+4//�����˲�ǿ��2
//#define  FF_Strong_Filter3  FF_Strong_Filter2+4//�����˲�ǿ��3
//#define  FF_Strong_Filter4  FF_Strong_Filter3+4//�����˲�ǿ��4
//////����_��λ��������ƽ��ֵ
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
	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//����NVIC��������ƫ�ƼĴ���
	//���ڱ�ʶ����������CODE��������RAM��
}

void MYRCC_DeInit(void)
{	
 	RCC->APB1RSTR = 0x00000000;//��λ����			 
	RCC->APB2RSTR = 0x00000000; 
	  
  	RCC->AHBENR = 0x00000014;  //˯��ģʽ�����SRAMʱ��ʹ��.�����ر�.	  
  	RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //�ر������ж�		 
	//����������				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else   
	MY_NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
#endif
}
 void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;   
	MYRCC_DeInit();		  //��λ������������
 	RCC->CR|=0x00010000;  //�ⲿ����ʱ��ʹ��HSEON
	while(!(RCC->CR>>17));//�ȴ��ⲿʱ�Ӿ���
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;//����2����λ
	RCC->CFGR|=PLL<<18;   //����PLLֵ 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	FLASH->ACR|=0x32;	  //FLASH 2����ʱ����

	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//�ȴ�PLL����
	RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��	 
	while(temp!=0x02)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	} 
}	
	   

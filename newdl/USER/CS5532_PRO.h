
/*CS5532*/
#define SET_CS()				GPIO_SetBits(GPIOD, GPIO_Pin_11)  		//GP4DAT = (GP4DAT | 0x00080000)	//P4.3->/CS
#define CLR_CS()				GPIO_ResetBits(GPIOD, GPIO_Pin_11)		//GP4DAT = (GP4DAT & 0xFFF7FFFF)

#define	SET_SCL()				GPIO_SetBits(GPIOB, GPIO_Pin_13)			//GP4DAT = (GP4DAT | 0x00100000)	//P4.4->SCLK
#define	CLR_SCL()				GPIO_ResetBits(GPIOB, GPIO_Pin_13)		//GP4DAT = (GP4DAT & 0xffefffff)

#define AD_SDI_1				GPIO_SetBits(GPIOB, GPIO_Pin_15)			//GP4DAT = (GP4DAT | 0x00200000)	//P4.5->DIN
#define AD_SDI_0				GPIO_ResetBits(GPIOB, GPIO_Pin_15)		//GP4DAT = (GP4DAT & 0xffdfffff)

#define AD_SDO                  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)

#define Read_Offset_Reg 0x09
#define Write_Offset_Reg 0x01

#define Read_Gain_Reg 0x0a
#define Write_Gain_Reg 0x02

#define Read_config 0x0B
#define Write_config 0x03

//u8 up_5513;

void Setting_CSR(char gain0,char OWR0,char CSR,char gain1,char OWR1);
u32 ReadADresult(void);
extern void EXTI_NVIC_Configuration(void);

#define start_AD_with_CSR00 0xc0+(0x00<<3)  //CSR 0
#define start_AD_with_CSR01 0xC0+(0x01<<3)  //CSR 1
#define start_AD_with_CSR02 0xC0+(0x02<<3)  //CSR 2
#define start_AD_with_CSR03 0xC0+(0x03<<3)  //CSR 3

#define start_AD_with_CSR04 0xC0+(0x04<<3)  //CSR 4
#define start_AD_with_CSR05 0xC0+(0x05<<3)  //CSR 5
#define start_AD_with_CSR06 0xC0+(0x06<<3)  //CSR 6
#define start_AD_with_CSR07 0xC0+(0x07<<3)  //CSR 7

#define Gain01 0
#define Gain02 1
#define Gain04 2 
#define Gain08 3
#define Gain16 4
#define Gain32 5
#define Gain64 6

#define Sps_120  0  
#define Sps_060  1 
#define Sps_030  2 
#define Sps_015  3
#define Sps_7_5  4
#define Sps_3840 8
#define Sps_1920 9
#define Sps_960  10
#define Sps_480  11
#define Sps_240  12
extern unsigned long AD_Result0;
extern 	u8 LED_LIGHTENX[]={0x01,0x00,0x00,0x00,0x0,0x0,0x0,0x0,0x0};
extern unsigned short CRC16(unsigned char *puchMsg, unsigned short usDataLen);
union AD_X1    
{    
long zero_point;
char zero_pointx[4];   
}AD_ZERO_STORE;

union AD_X2  
{    
float ad_coefficent; 
char ad_coeffx[4];   
}AD_COFF_STORE;


 void CS5532_GPIO_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG| RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF, ENABLE);
	   /*开启按键端口B的时钟*/
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;//CLK STB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//CLK STB0,EXTI_Line4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;   //  GPIO_Mode_IPU;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;   //  GPIO_Mode_IPU;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

}

void EXTI_Configuration(void)
{
//


EXTI_InitTypeDef EXTI_InitStructure;
  //配置外部中断4在下降沿时产生中断 
//	EXTI_DeInit();
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
  EXTI_InitStructure.EXTI_Line = 		EXTI_Line8;//
  EXTI_InitStructure.EXTI_Mode = 		EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =     EXTI_Trigger_Falling;//EXTI_Trigger_Rising_Falling;//
  EXTI_InitStructure.EXTI_LineCmd = 	ENABLE;
  EXTI_Init(&EXTI_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);
}

 




void AD_delay(unsigned int temp)  
{  
  unsigned int i,j;
  for(i=0;i<temp;i++)
  {
    for(j=0;j<10;j++);      
  }
}



void WRITEBYTE(unsigned char byte)
{
	unsigned char i,data;  
	data=byte;  
	for(i=0;i<8;i++)  
	{
		CLR_SCL();			
		AD_delay(10);//                  
		if((data&0x80)==0x80)AD_SDI_1;		
		else AD_SDI_0;		
		AD_delay(10);// 
		SET_SCL();			
		AD_delay(10);//         
		data <<= 1;  
	}
	CLR_SCL();   
}

unsigned char READBYTE(void)
{
	unsigned char i,data; 
	data = 0;
	for(i=0;i<8;i++)  
	{
		CLR_SCL();									//SET_SKM_H();          
        AD_delay(3);//          
		SET_SCL();
		AD_delay(3);//         
		data<<=1;  
		if(AD_SDO) data|=0x01;
	}
	CLR_SCL();		//这一句必须的，若无则AD码固定，在模拟量变化时，要复位后才出新的数据，且SDO波形转换周期(大部分时间为低电平)
	return(data);   //加了这一句，AD码随模拟量变化，读完数据SDO变为高电平，SDO(大部分时间为高电平)      
}

 void write_four_byte(unsigned long i_data)
 {
	unsigned char i;
    
	for(i=0;i<32;i++)
	{
		CLR_SCL();						//sclk=0;
		AD_delay(10);//
		if(i_data & 0x80000000) AD_SDI_1;
		else AD_SDI_0;
		SET_SCL();						//sclk=1;
		AD_delay(10);//
		i_data=i_data << 1;
	}
}

unsigned long read_four_byte(void)
{
	unsigned char i;
	long data = 0;
	for(i=0;i<32;i++)  
	{
		CLR_SCL();									//SET_SKM_H();          
        AD_delay(10);//          
		SET_SCL();
		AD_delay(10);//         
		data<<=1;  
		if(AD_SDO) data|=0x01;
	}
	return(data);         
}

void command(unsigned char command,unsigned long i_data)
{
		WRITEBYTE(command);	
		write_four_byte(i_data);
}
unsigned long read_register(unsigned char command)
{
		unsigned long o_data1;
		WRITEBYTE(command);	
		o_data1=read_four_byte();
		AD_SDI_0;											//sdi=0;
		CLR_SCL();											//sclk=0;
        AD_delay(10);//
		return (o_data1);
}
 /*******************************************
*初始化5532的串行接口*
*原型：init_cs553()
*
********************************************/
bool init_cs5532()
{
#define CYCN1 0xff
#define CYCN0 0xfe
#define Read_config 0x0B
#define Write_config 0x03

#define Rest_Chip   0x20000000
#define Rest_Chip_1 0x00000000
unsigned long temp;
char i;


	AD_SDI_0;
	CLR_SCL();
	CLR_CS();

	AD_delay(1000);//1000

	for(i=0;i<15;i++)
	{                              //26
	 	WRITEBYTE(CYCN1);
	}
	WRITEBYTE(CYCN0);
    AD_delay(10);//
/*
configuration
D31 D30 D29 D28 D27 D26 D25 D24 D23 D22 D21 D20 D19 D18 D17 D16
PSS PDW  RS  RV  IS  GB  VRS A1  A0  OLS  NU OGS FRS  NU  NU NU
D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
NU NU NU NU NU NU NU NU NU NU NU NU NU NU NU NU
*/
	command(Write_config,Rest_Chip);//写入CS5532复位命令 configuration
	temp=0;
	temp=read_register(Read_config);//读取CONFIGURATION REG 寄存器内容
//	command(Write_config,Rest_Chip_1);//写入CS5532复位命令 configuration
	if(temp&0x10000000)
	{
		temp=read_register(0x09);//read OFFSET
	 	temp=read_register(0x0A);//read GAIN
		temp=read_register(Read_config);//读取CONFIGURATION REG 寄存器内容 ox0b
//		temp=read_register(0x0d);//read CSR
//		Setting_CSR(Gain64,Sps_060,0,Gain64,Sps_060);

//		Setting_CSR(Gain64,Sps_7_5,0,Gain64,Sps_240);
//		Setting_CSR(Gain64,Sps_015,0,Gain64,Sps_240);


		Setting_CSR(Gain64,Sps_030,0,Gain64,Sps_240);
		temp=read_register(0x0d);//read CSR
// 		temp=read_register(0x0d);//read CSR	 

		WRITEBYTE(0xc0);//启动ADC
//		EXTI_NVIC_Configuration(); //启动EXTI 读AD
		EXTI_Configuration();
        CLR_SCL();
		return 1;
	}
	else
	return 0; 
}


void Read_ADC(void)
{
//unsigned long temp;
//		if(!AD_SDO)
//		 {temp=ReadADresult();
//		  }

}
void read_SCR(char CSR)
{
char commandx;
unsigned long contents;
	commandx=CSR;
	commandx<<=4;
	commandx|=0x0d;

contents=read_register(commandx);
}
void Setting_CSR(char gain0,char OWR0,char CSR,char gain1,char OWR1)
{
char commandx;
union SCR_reg    
{    
signed long reg; 
unsigned char L_reg[4];   
}SCR_reg0;
/*Setting CSR1*/ 
SCR_reg0.L_reg[3]=0;
SCR_reg0.L_reg[3]+=gain0;
SCR_reg0.L_reg[3]<<=3;
SCR_reg0.L_reg[3]|=(OWR0>>1);

SCR_reg0.L_reg[2]=(OWR0&0x01); 
SCR_reg0.L_reg[2]<<=7;
//SCR_reg0.L_reg[2]|=0x40;


SCR_reg0.L_reg[1]=0;
SCR_reg0.L_reg[1]+=gain1;
SCR_reg0.L_reg[1]<<=3;
SCR_reg0.L_reg[1]|=(OWR1>>1);

SCR_reg0.L_reg[0]=(OWR1&0x01); 
SCR_reg0.L_reg[0]<<=7;
//SCR_reg0.L_reg[0]|=0x40;

	commandx=CSR;
	commandx<<=4;
	commandx|=0x05;
	command(commandx,SCR_reg0.reg);//写入CS5532复位命令 configuration
/*
D7 D6 D5  D4  D3  D2 D1 D0
0  0  CS1 CS0 R/W 1 0 1
*/
/*
channel setting register
D31 D30 D29 D28 D27 D26 D25 D24 D23 D22 D21 D20 D19 D18 D17 D16
CS1 CS0 G2  G1  G0  WR3 WR2 WR1 WR0 U/B OL1 OL0 DT  OCD OG1 OG0
D15 D14 D13 D12 D11 D10 D9  D8  D7  D6  D5  D4  D3 D2  D1  D0
CS1 CS0 G2  G1  G0  WR3 WR2 WR1 WR0 U/B OL1 OL0 DT OCD OG1 OG0
important
G2  G1  G0
WR3 WR2 WR1

READ/WRITE INDIVIDUAL CHANNEL-SETUP REGISTER
Function: These commands are used to access each channel-setup register separately. CS1 - CS0 decode
the registers accessed.
R/W (Read/Write)
0 Write to selected register.
1 Read from selected register.

CS[1:0] (Channel Select Bits)
00 Channel-Setup Register 1 (All devices)
01 Channel-Setup Register 2 (All devices)
10 Channel-Setup Register 3 (All devices)
11 Channel-Setup Register 4 (All devices)
READ/WRITE


*/

}
void EXIT_AD(void)
{
#define CYCN1 0xff
	AD_SDI_1;
	WRITEBYTE(CYCN1);
}
void Setting_AD(void)
{
/*
Command Register Quick Reference

Command:read and write reg
D7 D6  D5  D4  D3  D2   D1   D0
0  ARA CS1 CS0 R/W RSB2 RSB1 RSB0
D2-D0 Register Select Bit,
RSB3-RSB0
000	Reserved
001	Offset Register
010	Gain Register
011	Configuration Register
101	Channel-Setup Registers
110 Reserved
111	Reserved
*/

/*
Command:start AD convert (single and contiune)
D7 D6 D5    D4    D3    D2  D1  D0
1  MC CSRP2 CSRP1 CSRP0 CC2 CC1 CC0
D2-D0 Conversion/Calibration Bits, CC2-CC0
D5-D3 Channel-Setup Register Pointer Bits,
CSRP
*/


WRITEBYTE(start_AD_with_CSR00);
}

u32 ReadADresult(void)
{
	unsigned char i; int px0;
union AD_packed    
{    
signed long weight_L; 
unsigned char L_weight[4];   
}AD_weight; 
	AD_SDI_0;CLR_SCL();
//	for(i=0;i<8;i++)
//	{
//		AD_delay(3);// 
//		SET_SCL();										      
//		AD_delay(3);//         
//		CLR_SCL();								
//	}
                       i=     READBYTE(); 
	AD_weight.L_weight[2] = READBYTE();	//先[2][1][0]
	AD_weight.L_weight[1] = READBYTE();	
	AD_weight.L_weight[0] = READBYTE();
	AD_weight.L_weight[3] = READBYTE();	//必须读四字节，少一个字节CS5532就会退出转换状态	 
	AD_weight.weight_L>>=3;

/**send AD code to Main board**/
			px0=CRC16((unsigned char *)AD_weight.L_weight,4);
/*
 			USART_SendData(USART2,0xEF);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			USART_SendData(USART2,AD_weight.L_weight[3]);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			USART_SendData(USART2,AD_weight.L_weight[2]);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			USART_SendData(USART2,AD_weight.L_weight[1]);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			USART_SendData(USART2,AD_weight.L_weight[0]);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

			USART_SendData(USART2,px0>>8);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			USART_SendData(USART2,px0&0x0ff);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

  			USART_SendData(USART2,0x00);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
*/
 /**send AD code to Main board**/

	return AD_weight.weight_L;
//union AD_packed    
//{    
//signed long weight_L; 
//unsigned char L_weight[4];   
//}AD_weight; 
// 	AD_SDI_0;
// 	AD_weight.weight_L=read_four_byte();
//	AD_weight.weight_L&=0x00ffffff;
//	AD_weight.L_weight[3]=0; //第一个8 SCLKs Clear SDO Flag
//	AD_weight.weight_L>>= 3;//get free noisz resoluton
//	return AD_weight.weight_L;
}

/*******************************************
*向CS5532寄存器写入命令****************
*原型command_cs5532*****************8
*
********************************************/

/*******************************************
*由CS5532寄存器读出数据        *************
*原型read_register_cs5532*****************8
*
********************************************/



 /**/
//void Init_Ad7190(void)
//{
//	unsigned long temp; unsigned char c;
////	initialize_filter();
//
////	output_ptr=0;
////	input_ptr = 0;
//		c=0;
//        AD_SDI_0;
//        AD_delay(10);
//        CLR_SCL();
//        AD_delay(10);
//		CLR_CS();
//		AD_delay(1000);//1000
//	do
//	{
//		init_cs5532();//初始化cs5532,串行口为命令状态
///*
//D31(MSB) D30 D29 D28 D27 D26 D25 D24 D23 D22 D21 D20 D19 D18 D17 D16
//PSS      PDW  RS  RV  IS  GB  VRS A1  A0  OLS  NU OGS FRS  NU  NU NU
//D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
//NU NU NU NU NU NU NU NU NU NU NU NU NU NU NU NU
//*/
//		command_cs5532(0x03,0x20000000);//写入CS5532复位命令 configuration
////		command_cs5532(0x03,0x20080000);//写入CS5532复位命令
//		temp=read_register_cs5532(0x0B);//读取CONFIGURATION REG 寄存器内容
//		
//		temp=temp&0x000000f0;   //0x000000F0
//		c++;
//        
//		if(c>10)//连续十次提示ADCERR
//		{
//        	WRITEBYTE(0xc0); 
//		while(1);//	c=0;
//		}
//	}
//	while(temp!=0x00000010);
//
//	WRITEBYTE(0x03);  
//	WRITEBYTE(0x00);  
//	WRITEBYTE(0x00);
//	
//	WRITEBYTE(0x58);  // 0x60 = 240; 0x58 = 480; 0x50 = 960; 0x48 = 1920; 0x40 = 3840
//	  				  //0x18 =15  0x20 =7.5 0x60 =240    0x = 0x = 
//	WRITEBYTE(0x00);
////	WRITEBYTE(0x05);  
////	WRITEBYTE(0x36);    // 0x35 = 480 ;  0x35 = 960 ; 0x36 = 240 ; 0x30 = 120 
////	WRITEBYTE(0x00);    // 0x80 = 480 ;  0x00 = 960 ; 0x00 = 240 ; 0x00 = 120
////	WRITEBYTE(0x36);    // 0x35 = 480 ;  0x35 = 960 ; 0x36 = 240 ; 0x30 = 120
////	WRITEBYTE(0x00);    // 0x80 = 480 ;  0x00 = 960 ; 0x00 = 240 ; 0x00 = 120
//	               
//	WRITEBYTE(0xc0); 
// }
//
//u32 ReadAD7190(void)
//{
////	unsigned char data1, data2, data3;
////	unsigned short i;
////	signed long InitADcount=0;
//union AD_packed    
//	{    
//	signed long weight_L; 
//	unsigned char L_weight[4];   
//	}AD_weight; 
// 
// 	AD_weight.weight_L=read_four_byte_cs5532_1();
//	AD_weight.weight_L&=0x00ffffff;
//	AD_weight.L_weight[0]=0; //第一个8 SCLKs Clear SDO Flag
//	AD_weight.weight_L>>= 3;//get free noisz resoluton
////
////
////
////	for(i=0;i<8;i++)
////	{
////		AD_delay(10);// 
////		SET_SCL();										//SET_SKM_H();        
////		AD_delay(10);//         
////		CLR_SCL();									//SET_SKM_L();
////	}  
////	
////	AD_weight.L_weight[1]=READBYTE();
////	AD_weight.L_weight[2]=READBYTE();
////	AD_weight.L_weight[3]=READBYTE();
////	InitADcount=AD_weight.weight_L;
////
////	data1 = READBYTE();	
////	data2 = READBYTE();	
////	data3 = READBYTE();	
////	READBYTE();
////	InitADcount = data1;
////	InitADcount = (InitADcount << 8) + data2; 
////	InitADcount = ((InitADcount << 8) + data3) << 8;
////	InitADcount = (InitADcount >> 11);
// }
// carlibration_step
void cal_operation(void)
{
 switch (carlibration_step)
 {
 	case 1:
			write_LED_Block(noLOAd,0,LED_LIGHTENX);
			zero_point=AD_Result0;
		break;
	case 2:
			write_LED_Block(ADLOAd,0,LED_LIGHTENX);
		break;
	case 3:
			write_LED_Block(caculate,0,LED_LIGHTENX);
			ad_coefficent=((float)(AD_Result0-zero_point))/1000.0000;
//			AD_COFF_STORE.ad_coefficent=ad_coefficent;
			carlibration_step=3;
		break;


	default:
		break;
}

}

 void _ltoa( char *p, long wt)
{
	unsigned int i;
	char *q,sign;

	sign=0;
	if(wt<0)
	{
	 wt=-wt;
	 sign=1;
    }

	q = p + 17;
	*--q = '\0';
	do
	{
	  i = (wt % 10) + '0';
	  if( i > '9' )
	    i+='A'-'0'-10;
	  *--q = i;
	} while( wt /= 10 );

    if(sign)
	   *p++ =0x16;//'-';  //负数加符号
	for(i=0;i<16;i++)
	   *p++ = *q++;	       
}
//	_ltoa(wttemp, wt);




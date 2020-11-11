//written by JY-MCU,QQ:179383020

//Contact:QQ:179383020,Website:http://jy-mcu.taobao.com
#ifndef	_TM1638_H
#define	_TM1638_H


#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0
#define delay_value	    150

//引脚定义
//sbit   DIO=P1^7;
//sbit   CLK=P1^6;
//sbit   STB=P1^5;

unsigned char  tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,\
/*      字形           0     1     2                                      a   b    c    d     e    f   */
					  0x3f,0x3E,0x31,0x6E,0x1C,0x54,0x40,0x03,    0x18,     0x38,0x73,0x5C,0x37,0x67,0x80,0x50,\
/*      字形           '0'   U  倒L    y     u   n    -  SY上引号  XY下引号   L   P    o    DM门  qq  dot   r*/

/*                     0x10 0x11  12  13    14                 18                               0x1f*/
					  0x08,0x53,0x58,0x04,0x76,0x78			,0x00
/*                    0x20  21   22   23   24   25            dim     */
/*      字形          xh:_  p2_   xc   BB   H     t
                            '2'少1画 */
                      };

/*
字形测试
{0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x0};
{0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1e,0x1f,0x0};
{0x20,0x21,0x22,0x23,0x24,0x25,0x0};
*/
#define	O 0x10 	 
#define U  0x11   
#define Q  U+1 //倒L
//#define +1
#define y  Q+1
#define u  y+1
#define n  u+1
#define _  n+1	//上横
#define SY _+1
#define XY SY+1
#define L XY+1
#define P L+1
#define o P+1
#define DM o+1
#define qq DM+1
#define dot qq+1
#define r dot+1
#define xh_ r+1
#define p2_ xh_+1
#define xc p2_+1
#define BB xc+1
#define H  BB+1
#define t H+1
#define dim t+1

 
 

unsigned char noLOAd[]={0x15,0x1b,0x19,0x10,0x0a,0x0d,0x0};//结束符号
unsigned char ADLOAd[]={0x0a,0x0d,0x19,0x10,0x0a,0x0d,0x0};
unsigned char caculate[]={0x0c,0x0a,0x22,0x14,0x19,0x0a,0x25,0x0e,0x0};

unsigned char OUQ[]={O,U,Q,0x0};
unsigned char yun_[]={y,u,n,_,0x0};
unsigned char SYXYLPoDMqqdot[]={SY,XY,L,P,o,DM,qq,dot,0x0};
unsigned char rxh_p2_xcBBHt[]={r,xh_,p2_,xc,BB,H,t,0x0};

unsigned char digtal_x[]={'1','2','3',0x0};

unsigned char LED_display_buffer[9];


void combinate_letter_digtal(unsigned char letter[],unsigned char digtal[])
{
   char letter_len,digtal_len,i;
  letter_len=strlen(letter);digtal_len=	strlen(digtal);
    for(i=0;i<9;i++)
	  {
	      LED_display_buffer[i]=dim;  //LED先全黑
	  }
	  LED_display_buffer[8]= 0;// 末位用0，以便测字符串长度
  for(i=0;i<8;i++)
  {
  	  if(i<letter_len)
	  {
	   	LED_display_buffer[i]=letter[i];//字符串显示	
	  }
	  if(i>=8-digtal_len)
	  {
	    LED_display_buffer[i]= digtal[i-(8-digtal_len)];  //数字串显示
	  }
  
  }

}


char flashed,flashing,flash_ponter,flash_ponter_image;
unsigned char *original;
void write_LED_Block(unsigned char data_str[],unsigned char point,unsigned char LED_LIGHTEN[]);
extern 	u8 LED_LIGHTENX[9];//={0x01,0x02,0x01,0x03,0x0,0x0,0x0,0x0,0x0};
//u8 LED_LIGHTENX[9];//={0x01,0x02,0x01,0x03,0x0,0x0,0x0,0x0,0x0};

void do_flashing(unsigned char flash_ponter)
{
   unsigned char flash_bufffer[9],original_bufffer[9],i;
   if (!flashing)return;
   for(i=0;i<9;i++)
   {
	original_bufffer[i]=*original;original++;
   }
   if(flashed)
   {

		write_LED_Block(original_bufffer,0,LED_LIGHTENX);
		flashed=0;
   }
   else
   {
		for(i=0;i<9;i++)
		{
			   if (i!=8-flash_ponter)
				   	{
					  flash_bufffer[i] =original_bufffer[i];
				   	}
			   else
					{
					  flash_bufffer[i] =dim;
					}
		} 

		write_LED_Block(flash_bufffer,0,LED_LIGHTENX);
		flashed=1;
   }
}


long zero_point;
float ad_coefficent;	
unsigned char carlibration_step;
void TM1638_Write(unsigned char	DATA)			//写数据函数
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		CLK_low;//CLK=0;
		Delay(delay_value);
		if(DATA&0X01)
			DO_high;//DIO=1 ;
		else
			DO_low;//DIO=0 ;
		DATA>>=1;
		CLK_high;//CLK=1 ;
		Delay(delay_value);
	}
}
unsigned char TM1638_Read(void)					//读数据函数
{
	unsigned char i;
	unsigned char temp=0;
	DO_high;//DIO=1 ;	//设置为DIO为高
//	 	Delay(delay_value);
	for(i=0;i<8;i++)
	{
 		temp>>=1;
		CLK_low;//CLK=0;
		Delay(delay_value);
#if defined	   _2803A
		if(!DI_IN)//  if(DIO)
#else
		if(DI_IN)//  if(DIO)
#endif
			temp|=0x80;
		CLK_high;//CLK=1 ;
		Delay(delay_value);
	}
	return temp;
}
void Write_COM(unsigned char cmd)		//发送命令字
{
	STB_low0;//STB=0 ;
	Delay(delay_value);
	TM1638_Write(cmd);
	STB_high0;//STB=1 ;
	Delay(delay_value);
}
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
	STB_low0;//STB=0 ;
 	Delay(delay_value);
	TM1638_Write(0x42);
	for(i=0;i<4;i++)
		c[i]=TM1638_Read();
	STB_high0;//STB=1 ;					//4个字节数据合成一个字节
	Delay(delay_value);
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i)==key_value)
			break;
	return i;
}
void Write_DATA(unsigned char add,unsigned char DATA)		//指定地址写入数据
{
	Write_COM(0x44);
	STB_low0;//STB=0 ;
//	Delay(delay_value);
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
//	Delay(delay_value);
	STB_high0;//STB=1 ;
}
void Write_oneLED(unsigned char num,unsigned char flag)	//单独控制一个LED函数，num为需要控制的led序号，flag为0时熄灭，不为0时点亮
{
	Write_DATA(2*num+1,flag&0x03);
}
/*void Write_allLED(unsigned char LED_flag)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i=0;i<8;i++)
		Write_DATA(2*i+1,LED_flag&0x03);
}	 */
void init_TM1638(void)
{
unsigned char i,j,p;
//STB_low0;
//for(i=0;i<100;i++);
//STB_high0;
//	p=Read_key();
//	if(p<8)goto display_led;
for(j=0;j<16;j++)
{ 
//	p=Read_key();
//	if(p>=8)return; 

/****写指示灯 0xc1*********/
	Write_COM(0x40);
	STB_low0;//STB=0 ;
	Delay(delay_value);
	TM1638_Write(0xc1);	//地址，确定哪个LED灯(对)亮	//seg9 10 为奇地址
	TM1638_Write(3);//双色亮  两个都亮
	STB_high0;//STB=1 ;
	Delay(delay_value);
	Write_COM(0x8e);//亮度
	 Delay(5000000);

	STB_low0;//STB=0 ;
	Delay(delay_value);
	TM1638_Write(0xc1);
	TM1638_Write(2);//单色亮
	STB_high0;//STB=1 ;
	Delay(delay_value);
	Write_COM(0x8e);//亮度
	 Delay(5000000);

	STB_low0;//STB=0 ;
	Delay(delay_value);
	TM1638_Write(0xc1);
	TM1638_Write(1);//单色亮
	STB_high0;//STB=1 ;
	Delay(delay_value);
	Write_COM(0x8c);//亮度
	Delay(5000000);

	STB_low0;//STB=0 ;
	Delay(delay_value);
	TM1638_Write(0xc1);
	TM1638_Write(00);//双色灭	两个都灭
	STB_high0;//STB=1 ;
	Delay(delay_value);
	Write_COM(0x8c);//亮度
	 Delay(5000000);
/**************************************/
#if defined	   _2803A
	p=Read_key();
#else
//	TM1683_IN();
	p=Read_key();
//	TM1683_OUT();
#endif
/**************************************/

	if(p<8)goto display_led;

	Write_COM(0x40);
	STB_low0;//STB=0 ;
	Delay(delay_value);
	TM1638_Write(0xc0+p*2);//seg1-8 为偶地址
	for(i=0;i<16;i++)
	  {
	    if(j==2||j==7)
		TM1638_Write(tab[j]+0x80); //数据
		else
		TM1638_Write(tab[j]);
	  } 
	STB_high0;//STB=1 ;
	Delay(delay_value);
		Write_COM(0x89);//亮度

	STB_high0;//STB=1 ;
	Delay(delay_value);
		Write_COM(0x89);//亮度

   Delay(5000000);

}

 return; 












 display_led:
	if(p>=8)return; 
 for(j=0;j<8;j++)
{ 

	Write_COM(0x40);
	STB_low0;//STB=0 ;
	Delay(delay_value);
	TM1638_Write(0xc0+p*2);

	switch (p)
	{
 		case 0:
				 TM1638_Write(tab[p]);
			break;
		case 1:
				 TM1638_Write(tab[p]);
			break;
		case 2:
				 TM1638_Write(tab[p]);
			break;
		case 3:
				 TM1638_Write(tab[p]);
			break;
		case 4:
				 TM1638_Write(tab[p]);
			break;
		case 5:
				 TM1638_Write(tab[p]);
			break;
		case 6:
				 TM1638_Write(tab[p]);
			break;
		case 7:
				 TM1638_Write(tab[p]);
			break;

		default:
			break;
	}

	STB_high0;//STB=1 ;
	Delay(delay_value);
		Write_COM(0x89);//亮度

   Delay(5000000);

}
}

void write_LED_Block(unsigned char data_str[],unsigned char point,unsigned char LED_LIGHTEN[])
{
/*
以8位LED为显示单元：小数点显示在第8-point-1  位上  point 小数点位数
对于从左的(i/2)<(8-lenx)位LED灭(lenx为显示数据的长度)
8-lenx：是数据显示的位数
数据数组为data_strx0[]每个数组元素对应着一位LED块（ASCII：0--F）



LED_LIGHTEN[]数组元素 俩位有效(四种组合)
如：LED_LIGHTENX[1]定义为“稳定灯”那么，
#define stab_led_on   LED_LIGHTENX[1]=0x01;	绿灯
#define stab_led_off   LED_LIGHTENX[1]=0x02; 红灯
下面就是执行写LED块，小数点，和根据LED_LIGHTENX数组点亮对应的指示灯
stab_led_on;
write_LED_Block(data_strx1,2,LED_LIGHTENX);
stab_led_off;
write_LED_Block(data_strx1,4,LED_LIGHTENX);

***************************************************
数字必须为ASCII，因为显示字符串与0会忽略0后面的字符串	 这是因为用了strlen的缘故
发黑位用dim
文本用本文件的tab表里的 字形
*/


unsigned char i,lenx;
	lenx=strlen(data_str);
	for(i=0;i<lenx;i++)
	{
	  if((data_str[i]<0x40)&&(data_str[i]>=0x30))
	  {
	  	data_str[i]&=0x0f;
	  }
//	  else
//	   {
//	    data_str[i]-='a';
//		data_str[i]+=10;
//	   }
	}


	Write_COM(0x40);
	STB_low0;//STB=0 ;
	Delay(delay_value);
//	TM1638_Write(0xc0+(8-lenx)*2);//seg1-8 为偶地址
	TM1638_Write(0xc0);//seg1-8 为偶地址

	for(i=0;i<16;i++)//2*lenx;i++)
	  {
	   	if(!(i%2))
		  {
		     if((i/2)>=(8-lenx))
			   {
				    if(point)
					{	//数据地址0 2 4 6 8 10 12 14 
						if((i/2)==(8-point-1))//(lenx-point-1))
							TM1638_Write(tab[data_str[i/2-(8-lenx)]]+0x80); //小数点
						else
							TM1638_Write(tab[data_str[i/2-(8-lenx)]]); //有	小数点时，不含小数点的位
					}
					else
					TM1638_Write(tab[data_str[i/2-(8-lenx)]]);//无小数点的显示
			   }
			else //灭没有显示的前几位LED
			   {TM1638_Write(0);}
		  }
	    else
		  {TM1638_Write(LED_LIGHTEN[i/2]);} //指示灯地址 1 3 5 7 9 11 13 15


	  } 
		STB_high0;//STB=1 ;
		Delay(delay_value);
		Write_COM(0x89);//亮度
}


#endif

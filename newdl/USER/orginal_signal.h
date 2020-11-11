#include "Key.h" 
#include "tm1638.h"

unsigned char num[8],led_flag[8];		//各个数码管显示的值


#define Timer_num 50
#define Counter_num 50
#define Weight_num 50

#define X_num 50
#define Y_num 50
#define R_num 50
#define False  0
#define True   1 

extern char face_in_out_been_opened;
char Modbus_input_buffer[2],Modbus_output_buffer[2];

char  X_input_counter[X_num];
char  X_input_state[X_num];
char  Y_output_state[Y_num];	   //bool
bool  R_relay_state[R_num];

int  T_timer_count[Timer_num];
int  T_timer_value[Timer_num];//* 10
bool T_timer_trigger[Timer_num];
bool T_timer_state[Timer_num];

int   C_counter_count[Counter_num];
int   C_counter_value[Counter_num]; 
bool  C_counter_state[Counter_num];
bool  C_counter_trigger[Counter_num];

int Weight_value;// net weight =Gross_weight-Tare_Weight
int Weight_control_value[Weight_num];
//bool Weight_value_checking[Weight_num];
bool Weight_control_state[Weight_num];
char Weight_control_requirement[Weight_num];

int Step_value;// net weight =Gross_weight-Tare_Weight
int Step_control_value[Weight_num];
//bool Weight_value_checking[Weight_num];
bool Step_control_state[Weight_num];
char Step_control_requirement[Weight_num];
u8 W_Totall,S_Totall;

/*for detect edage*/
bool     X_Late_Status[X_num];
bool     T_Late_Status[Timer_num];
bool     C_Late_Status[Counter_num];
bool     R_Late_Status[R_num];
bool     W_Late_Status[Weight_num];






void Sampling_X_input(void)
{
 X_input_state[0]=(bool)KEY0;
 X_input_state[1]=(bool)!KEY2;
}

void Store_XRCT_Status(void)
{
u8 k;
	for(k=0;k<50;k++)
	{
	    X_Late_Status[k]=X_input_state[k];
	    T_Late_Status[k]=T_timer_state[k];
	    C_Late_Status[k]=C_counter_state[k];
	    R_Late_Status[k]=R_relay_state[k];
	    W_Late_Status[k]=Weight_control_state[k];
	
	}
}


 
void Weight_checking(int k)
{
	Weight_control_state[k]=False;
	switch (Weight_control_requirement[k])
	{
		case '=':
			if(Weight_value==Weight_control_value[k])
				Weight_control_state[k]=True;
			break;
		case '>':
			if(Weight_value>Weight_control_value[k])
				Weight_control_state[k]=True;
			break;
		case '<':
			if(Weight_value<Weight_control_value[k])
				Weight_control_state[k]=True;
			break;
		default:
			break;
	}

}

void Step_checking(int k)
{
	Step_control_state[k]=False;
	switch (Step_control_requirement[k])
	{
		case '=':
			if(Step_value==Step_control_value[k])
				Step_control_state[k]=True;
			break;
		case '>':
			if(Step_value>Step_control_value[k])
				Step_control_state[k]=True;
			break;
		case '<':
			if(Step_value<Step_control_value[k])
				Step_control_state[k]=True;
			break;
		default:
			break;
	}

}

bool Detect_edge(char edge ,char XRCT, unsigned char k )
{
bool logic;
	 switch (XRCT)
	 {
	 	case 'X':
			 switch (edge)
			 {
			 	case '='://"‖"上升沿和下降沿
				      logic=X_Late_Status[k]^X_input_state[k];
				break;
				case '>'://"↑"上升沿
				      logic=~X_Late_Status[k]&X_input_state[k];
				break;
				case '<'://"↓" 下降沿
				      logic=X_Late_Status[k]&~X_input_state[k];
				break;
				default:
				break;
			}
		break;
		case 'R': 
			 switch (edge)
			 {
			 	case '='://"‖"上升沿和下降沿
				      logic=R_Late_Status[k]^R_relay_state[k];
				break;
				case '>'://"↑"上升沿
				      logic=~R_Late_Status[k]&R_relay_state[k];
				break;
				case '<'://"↓" 下降沿
				      logic=R_Late_Status[k]&~R_relay_state[k];
				break;
				default:
				break;
			}
		break;		   
	 	case 'C':
			 switch (edge)
			 {
			 	case '='://"‖"上升沿和下降沿
				      logic=C_Late_Status[k]^C_counter_state[k];
				break;
				case '>'://"↑"上升沿
				      logic=~C_Late_Status[k]&C_counter_state[k];
				break;
				case '<'://"↓" 下降沿
				      logic=C_Late_Status[k]&~C_counter_state[k];
				break;
				default:
				break;
			}		
		break;
		case 'T':
			 switch (edge)
			 {
			 	case '='://"‖"上升沿和下降沿
				      logic=T_Late_Status[k]^T_timer_state[k];
				break;
				case '>'://"↑"上升沿
				      logic=~T_Late_Status[k]&T_timer_state[k];
				break;
				case '<'://"↓" 下降沿
				      logic=T_Late_Status[k]&~T_timer_state[k];
				break;
				default:
				break;
			}
		break;
	 	case 'W':	
			 switch (edge)
			 {
			 	case '='://"‖"上升沿和下降沿
				      logic=W_Late_Status[k]^Weight_control_state[k];
				break;
				case '>'://"↑"上升沿
				      logic=~W_Late_Status[k]&Weight_control_state[k];
				break;
				case '<'://"↓" 下降沿
				      logic=W_Late_Status[k]&~Weight_control_state[k];
				break;
				default:
				break;
			}	
		 break;







		default:
			break;
	}

	return logic;
}

void Timer_reset(u8 k)
{
        T_timer_trigger[k]=False;
        T_timer_count[k]=0;
        T_timer_state[k]=False;
}


void Timer_trigger(u8 k)
{
	 if(!T_timer_trigger[k])
	 	{
		  T_timer_count[k]=T_timer_value[k];//初始化定时器
//		  T_timer_count[k]*=10;
		  T_timer_trigger[k]=True;
		}

}
void Timer_run(u8 k)
{
        
	if(T_timer_count[k])
		{
             T_timer_count[k]--;
             if(T_timer_count[k]==1)
                    {T_timer_state[k]=True;}
		}
}

void Counter_reset(u8 k) //'计数器初始复位 用于计数器/C输出的逻辑运算为 真
{
        C_counter_count[k]=0;
        C_counter_state[k]=False;
}
void Counter_running_reset(u8 k) //用于计数器的逻辑运算为 假   '计数器触发条件为 假
      {  C_counter_trigger[k]=False;	}
         
void Counter_trigger(u8 k) //用于计数器的逻辑运算为 真	 '当计数器触发条件为 真 
{
   if(!C_counter_trigger[k])
   	{
        if(C_counter_value[k]>0)//预置值>0
	         {  if(C_counter_count[k]<C_counter_value[k]-1)
	                {C_counter_count[k]++;//计数未到
//					 C_counter_state[k]=False; //清状态 
					 C_counter_trigger[k]=True;//'计数器被触发一次，下次触发必须在此标志复位后
				    }	       			       //接通一次+1，连续接通不加
			   else
			        { //计数到
					  C_counter_state[k]=True; //置状态
					  C_counter_count[k]=0;//清计数器
					}
			 }
               //预置值<=0
   }
                
}

void Test_input_output(void)
{


//	unsigned char i;
//		while(1)
//		{
////	    	if(DI_IN)//c5
////	         init_TM1638();
////		    else 
////			 i=0;
//	         init_TM1638();
//
//	    }


//	for(i=0;i<8;i++)Write_DATA(i<<1,tab[0]);
//	while(1)
//		{
//					i=1;
//					led_flag[i]=1;
//					Write_DATA(i*2,tab[num[i]]);//写第i个数码管
//					Write_COM(0x81);//亮度
//					Write_oneLED(i,led_flag[i]);//写第i个led状态，并且不改变其他led状态以方便对比
//
//
//			i=Read_key();
//			if(i<8)
//				{
//					num[i]++;led_flag[i]++;		//第i个数码管变量加1，led状态+1;
//					while(Read_key()==i);		//等待按键释放
//					if(num[i]>15)
//						num[i]=0;
//					if(led_flag[i]>3)
//						led_flag[i]=0;
//					Write_DATA(i*2,tab[num[i]]);//写第i个数码管
//					Write_oneLED(i,led_flag[i]);//写第i个led状态，并且不改变其他led状态以方便对比
//				}
//		}





//  Test_D:
//  		DO_high;
//		DO_low;//c3
//
//		CLK_high;//a5
//		CLK_low;
//
//		STB_high0;//a7
//	    STB_low0;
//
//// 		STB_high1;//e14
////	    STB_low1;
//		goto Test_D;
//		if(DI_IN){}//c5


// Test_Y:
//		 Y_output_high_00;
//		 Y_output_lowx_00;
//		
//		 Y_output_high_01;
//		 Y_output_lowx_01;
//		
//		 Y_output_high_02;
//		 Y_output_lowx_02;
//		
//		 Y_output_high_03;
//		 Y_output_lowx_03;
//		
//		 Y_output_high_04;
//		 Y_output_lowx_04;
//		
//		 Y_output_high_05;
//		 Y_output_lowx_05;
//		
//		 Y_output_high_06;
//		 Y_output_lowx_06;
//		
//		 Y_output_high_07;
//		 Y_output_lowx_07;
//		
//		 Y_output_high_08;
//		 Y_output_lowx_08;
//		
//		 Y_output_high_09;
//		 Y_output_lowx_09;
//		
//		 Y_output_high_0A;
//		 Y_output_lowx_0A;
//		
//		 Y_output_high_0B;
//		 Y_output_lowx_0B;
//		
//		 Y_output_high_0C;
//		 Y_output_lowx_0C;
//		
//		 Y_output_high_0D;
//		 Y_output_lowx_0D;
//		
//		 Y_output_high_0E;
//		 Y_output_lowx_0E;
//		
//		 Y_output_high_0F;
//		 Y_output_lowx_0F;
//
//
//		 goto  Test_Y;


		if(X_input_00)
			{Y_output_high_00;}
		else
			{Y_output_lowx_00;}
		
		if(X_input_01)
			{Y_output_high_01;}
		else
			{Y_output_lowx_01;}
		
		if(X_input_02)
			{Y_output_high_02;}
		else
			{Y_output_lowx_02;}
							
		if(X_input_03)
			{Y_output_high_03;}
		else
			{Y_output_lowx_03;}
		
		if(X_input_04)
			{Y_output_high_04;}
		else
			{Y_output_lowx_04;}
		
		if(X_input_05)
			{Y_output_high_05;}
		else
			{Y_output_lowx_05;}
							
		if(X_input_06)
			{Y_output_high_06;}
		else
			{Y_output_lowx_06;}
		
		if(X_input_07)
			{Y_output_high_07;}
		else
			{Y_output_lowx_07;}
		
		if(X_input_08)
			{Y_output_high_08;}
		else
			{Y_output_lowx_08;}
		
		if(X_input_09)
			{Y_output_high_09;}
		else
			{Y_output_lowx_09;}
		
		if(X_input_0A)
			{Y_output_high_0A;}
		else
			{Y_output_lowx_0A;}
		
		if(X_input_0B)
			{Y_output_high_0B;}
		else
			{Y_output_lowx_0B;}
		
		if(X_input_0C)
			{Y_output_high_0C;}
		else
			{Y_output_lowx_0C;}
		
		if(X_input_0D)
			{Y_output_high_0D;}
		else
			{Y_output_lowx_0D;}
		
		if(X_input_0E)
			{Y_output_high_0E;}
		else
			{Y_output_lowx_0E;}
		
		if(X_input_0F)
			{Y_output_high_0F;}
		else
			{Y_output_lowx_0F;}
/*拨盘*/
		if(!Command_01)
			{Y_output_high_00;}
		else
			{Y_output_lowx_00;}

		if(!Command_02)
			{Y_output_high_01;}
		else
			{Y_output_lowx_01;}

		if(!Command_03)
			{Y_output_high_02;}
		else
			{Y_output_lowx_02;}

		if(!Command_04)
			{Y_output_high_03;}
		else
			{Y_output_lowx_03;}



}
void Modbus_input(void)
{
	if (X_input_state[0])
	{Modbus_input_buffer[0]|=0x01;}
	else
	{Modbus_input_buffer[0]&=~(0x01);}

	if (X_input_state[1])
	{Modbus_input_buffer[0]|=0x02;}
	else
	{Modbus_input_buffer[0]&=~(0x02);}

	if (X_input_state[2])
	{Modbus_input_buffer[0]|=0x04;}
	else
	{Modbus_input_buffer[0]&=~(0x04);}

	if (X_input_state[3])
	{Modbus_input_buffer[0]|=0x08;}
	else
	{ Modbus_input_buffer[0]&=~(0x08);}

	if (X_input_state[4])
	{Modbus_input_buffer[0]|=0x10;}
	else
	{Modbus_input_buffer[0]&=~(0x10);}

	if (X_input_state[5])
	{Modbus_input_buffer[0]|=0x20;}
	else
	{Modbus_input_buffer[0]&=~(0x20);}

	if (X_input_state[6])
	{Modbus_input_buffer[0]|=0x40;}
	else
	{Modbus_input_buffer[0]&=~(0x40);}

	if (X_input_state[7])
	{ Modbus_input_buffer[0]|=0x80;}
	else
	{ Modbus_input_buffer[0]&=~(0x80);}
	/*****************************************/
	if (X_input_state[8])
	{Modbus_input_buffer[1]|=0x01;}
	else
	{Modbus_input_buffer[1]&=~(0x01);}

	if (X_input_state[9])
	{Modbus_input_buffer[1]|=0x02;}
	else
	{Modbus_input_buffer[1]&=~(0x02);}

	if (X_input_state[10])
	{Modbus_input_buffer[1]|=0x04;}
	else
	{Modbus_input_buffer[1]&=~(0x04);}

	if (X_input_state[11])
	{Modbus_input_buffer[1]|=0x08;}
	else
	{ Modbus_input_buffer[1]&=~(0x08);}

	if (X_input_state[12])
	{Modbus_input_buffer[1]|=0x10;}
	else
	{Modbus_input_buffer[1]&=~(0x10);}

	if (X_input_state[13])
	{Modbus_input_buffer[1]|=0x20;}
	else
	{Modbus_input_buffer[1]&=~(0x20);}

	if (X_input_state[14])
	{Modbus_input_buffer[1]|=0x40;}
	else
	{Modbus_input_buffer[1]&=~(0x40);}

	if (X_input_state[15])
	{ Modbus_input_buffer[1]|=0x80;}
	else
	{ Modbus_input_buffer[1]&=~(0x80);}

}

 void Modbus_output(void)
{
	if (Y_output_state[0])
	{Modbus_output_buffer[0]|=0x01;}
	else
	{Modbus_output_buffer[0]&=~(0x01);}

	if (Y_output_state[1])
	{Modbus_output_buffer[0]|=0x02;}
	else
	{Modbus_output_buffer[0]&=~(0x02);}

	if (Y_output_state[2])
	{Modbus_output_buffer[0]|=0x04;}
	else
	{Modbus_output_buffer[0]&=~(0x04);}

	if (Y_output_state[3])
	{Modbus_output_buffer[0]|=0x08;}
	else
	{ Modbus_output_buffer[0]&=~(0x08);}

	if (Y_output_state[4])
	{Modbus_output_buffer[0]|=0x10;}
	else
	{Modbus_output_buffer[0]&=~(0x10);}

	if (Y_output_state[5])
	{Modbus_output_buffer[0]|=0x20;}
	else
	{Modbus_output_buffer[0]&=~(0x20);}

	if (Y_output_state[6])
	{Modbus_output_buffer[0]|=0x40;}
	else
	{Modbus_output_buffer[0]&=~(0x40);}

	if (Y_output_state[7])
	{ Modbus_output_buffer[0]|=0x80;}
	else
	{ Modbus_output_buffer[0]&=~(0x80);}
	/*****************************************/
	if (Y_output_state[8])
	{Modbus_output_buffer[1]|=0x01;}
	else
	{Modbus_output_buffer[1]&=~(0x01);}

	if (Y_output_state[9])
	{Modbus_output_buffer[1]|=0x02;}
	else
	{Modbus_output_buffer[1]&=~(0x02);}

	if (Y_output_state[10])
	{Modbus_output_buffer[1]|=0x04;}
	else
	{Modbus_output_buffer[1]&=~(0x04);}

	if (Y_output_state[11])
	{Modbus_output_buffer[1]|=0x08;}
	else
	{ Modbus_output_buffer[1]&=~(0x08);}

	if (Y_output_state[12])
	{Modbus_output_buffer[1]|=0x10;}
	else
	{Modbus_output_buffer[1]&=~(0x10);}

	if (Y_output_state[13])
	{Modbus_output_buffer[1]|=0x20;}
	else
	{Modbus_output_buffer[1]&=~(0x20);}

	if (Y_output_state[14])
	{Modbus_output_buffer[1]|=0x40;}
	else
	{Modbus_output_buffer[1]&=~(0x40);}

	if (Y_output_state[15])
	{ Modbus_output_buffer[1]|=0x80;}
	else
	{ Modbus_output_buffer[1]&=~(0x80);}

}


void Input_Filter(void)
 {
 #define input_time 1
 		if(X_input_00)
			{X_input_counter[0]++;
			 if(X_input_counter[0]>=input_time)
			 	{X_input_counter[0]=0;
				 X_input_state[0]=True;
				}
			}
	   else
	   		{
			   X_input_counter[0]=0;
			   X_input_state[0]=False;
			}
//1+++++++++++++++++++++++++++++++++++++++
  		if(X_input_01)
			{X_input_counter[1]++;
			 if(X_input_counter[1]>=input_time)
			 	{X_input_counter[1]=0;
				 X_input_state[1]=True;
				}
			}
	   else
	   		{
			   X_input_counter[1]=0;
			   X_input_state[1]=False;
			}
//2+++++++++++++++++++++++++++++++++++++++
  		if(X_input_02)
			{X_input_counter[2]++;
			 if(X_input_counter[2]>=input_time)
			 	{X_input_counter[2]=0;
				 X_input_state[2]=True;
				}
			}
	   else
	   		{
			   X_input_counter[2]=0;
			   X_input_state[2]=False;
			}
//3+++++++++++++++++++++++++++++++++++++++
  		if(X_input_03)
			{X_input_counter[3]++;
			 if(X_input_counter[3]>=input_time)
			 	{X_input_counter[3]=0;
				 X_input_state[3]=True;
				}
			}
	   else
	   		{
			   X_input_counter[3]=0;
			   X_input_state[3]=False;
			}
//4+++++++++++++++++++++++++++++++++++++++
  		if(X_input_04)
			{X_input_counter[4]++;
			 if(X_input_counter[4]>=input_time)
			 	{X_input_counter[4]=0;
				 X_input_state[4]=True;
				}
			}
	   else
	   		{
			   X_input_counter[4]=0;
			   X_input_state[4]=False;
			}
//5+++++++++++++++++++++++++++++++++++++++
  		if(X_input_05)
			{X_input_counter[5]++;
			 if(X_input_counter[5]>=input_time)
			 	{X_input_counter[5]=0;
				 X_input_state[5]=True;
				}
			}
	   else
	   		{
			   X_input_counter[5]=0;
			   X_input_state[5]=False;
			}
//6+++++++++++++++++++++++++++++++++++++++
  		if(X_input_06)
			{X_input_counter[6]++;
			 if(X_input_counter[6]>=input_time)
			 	{X_input_counter[6]=0;
				 X_input_state[6]=True;
				}
			}
	   else
	   		{
			   X_input_counter[6]=0;
			   X_input_state[6]=False;
			}
//7+++++++++++++++++++++++++++++++++++++++
  		if(X_input_07)
			{X_input_counter[7]++;
			 if(X_input_counter[7]>=input_time)
			 	{X_input_counter[7]=0;
				 X_input_state[7]=True;
				}
			}
	   else
	   		{
			   X_input_counter[7]=0;
			   X_input_state[7]=False;
			}
//8+++++++++++++++++++++++++++++++++++++++
  		if(X_input_08)
			{X_input_counter[8]++;
			 if(X_input_counter[8]>=input_time)
			 	{X_input_counter[8]=0;
				 X_input_state[8]=True;
				}
			}
	   else
	   		{
			   X_input_counter[8]=0;
			   X_input_state[8]=False;
			}
//9+++++++++++++++++++++++++++++++++++++++
  		if(X_input_09)
			{X_input_counter[9]++;
			 if(X_input_counter[9]>=input_time)
			 	{X_input_counter[9]=0;
				 X_input_state[9]=True;
				}
			}
	   else
	   		{
			   X_input_counter[9]=0;
			   X_input_state[9]=False;
			}
//10+++++++++++++++++++++++++++++++++++++++
  		if(X_input_0A)
			{X_input_counter[10]++;
			 if(X_input_counter[10]>=input_time)
			 	{X_input_counter[10]=0;
				 X_input_state[10]=True;
				}
			}
	   else
	   		{
			   X_input_counter[10]=0;
			   X_input_state[10]=False;
			}
//11+++++++++++++++++++++++++++++++++++++++
  		if(X_input_0B)
			{X_input_counter[11]++;
			 if(X_input_counter[11]>=input_time)
			 	{X_input_counter[11]=0;
				 X_input_state[11]=True;
				}
			}
	   else
	   		{
			   X_input_counter[11]=0;
			   X_input_state[11]=False;
			}
//12+++++++++++++++++++++++++++++++++++++++
  		if(X_input_0C)
			{X_input_counter[12]++;
			 if(X_input_counter[12]>=input_time)
			 	{X_input_counter[12]=0;
				 X_input_state[12]=True;
				}
			}
	   else
	   		{
			   X_input_counter[12]=0;
			   X_input_state[12]=False;
			}
//13+++++++++++++++++++++++++++++++++++++++
  		if(X_input_0D)
			{X_input_counter[13]++;
			 if(X_input_counter[13]>=input_time)
			 	{X_input_counter[13]=0;
				 X_input_state[13]=True;
				}
			}
	   else
	   		{
			   X_input_counter[13]=0;
			   X_input_state[13]=False;
			}

//14+++++++++++++++++++++++++++++++++++++++
  		if(X_input_0E)
			{X_input_counter[14]++;
			 if(X_input_counter[14]>=input_time)
			 	{X_input_counter[14]=0;
				 X_input_state[14]=True;
				}
			}
	   else
	   		{
			   X_input_counter[14]=0;
			   X_input_state[14]=False;
			}
//15++++++++++++++++++++++++++++++++++++++
  		if(X_input_0F)
			{X_input_counter[15]++;
			 if(X_input_counter[15]>=input_time)
			 	{X_input_counter[15]=0;
				 X_input_state[15]=True;
				}
			}
	   else
	   		{
			   X_input_counter[15]=0;
			   X_input_state[15]=False;
			}
 
 }
void sent_x_state_to_y_state(void)
{
 Y_output_state[0]=X_input_state[0];
 Y_output_state[1]=X_input_state[1];
 Y_output_state[2]=X_input_state[2];
 Y_output_state[3]=X_input_state[3];
 Y_output_state[4]=X_input_state[4];
 Y_output_state[5]=X_input_state[5];
 Y_output_state[6]=X_input_state[6];
 Y_output_state[7]=X_input_state[7];
 Y_output_state[8]=X_input_state[8];
 Y_output_state[9]=X_input_state[9];
 Y_output_state[10]=X_input_state[10];
 Y_output_state[11]=X_input_state[11];
 Y_output_state[12]=X_input_state[12];
 Y_output_state[13]=X_input_state[13];
 Y_output_state[14]=X_input_state[14];
 Y_output_state[15]=X_input_state[15];

}

 void Y_relay_state(void)
{
		if(face_in_out_been_opened)return;
		if(Y_output_state[0])
			{Y_output_high_00;}
		else
			{Y_output_lowx_00;}
		
		if(Y_output_state[1])
			{Y_output_high_01;}
		else
			{Y_output_lowx_01;}
		
		if(Y_output_state[2])
			{Y_output_high_02;}
		else
			{Y_output_lowx_02;}
							
		if(Y_output_state[3])
			{Y_output_high_03;}
		else
			{Y_output_lowx_03;}
		
		if(Y_output_state[4])
			{Y_output_high_04;}
		else
			{Y_output_lowx_04;}
		
		if(Y_output_state[5])
			{Y_output_high_05;}
		else
			{Y_output_lowx_05;}
							
		if(Y_output_state[6])
			{Y_output_high_06;}
		else
			{Y_output_lowx_06;}
		
		if(Y_output_state[7])
			{Y_output_high_07;}
		else
			{Y_output_lowx_07;}
		
		if(Y_output_state[8])
			{Y_output_high_08;}
		else
			{Y_output_lowx_08;}
		
		if(Y_output_state[9])
			{Y_output_high_09;}
		else
			{Y_output_lowx_09;}
		
		if(Y_output_state[10])
			{Y_output_high_0A;}
		else
			{Y_output_lowx_0A;}
		
		if(Y_output_state[11])
			{Y_output_high_0B;}
		else
			{Y_output_lowx_0B;}
		
		if(Y_output_state[12])
			{Y_output_high_0C;}
		else
			{Y_output_lowx_0C;}
		
		if(Y_output_state[13])
			{Y_output_high_0D;}
		else
			{Y_output_lowx_0D;}
		
		if(Y_output_state[14])
			{Y_output_high_0E;}
		else
			{Y_output_lowx_0E;}
		
		if(Y_output_state[15])
			{Y_output_high_0F;}
		else
			{Y_output_lowx_0F;}



}

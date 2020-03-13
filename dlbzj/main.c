/******************************************************************************/
/*                                                                            */
/*                     xi'an ziping pack machine factory                     */
/*                           dlbzj 5.6 copyright@2014 By John              */
/******************************************************************************/
#include <intrins.h>
#include "stc12cxx.h"
//#include "iap.h"
#include "24lc16.h"
#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */
#include "mydefine.h"
#include <stdio.h>
//------------modbus------------------------------
#include "mb.h"
#define REG_HOLDING_START   	1	//Hold寄存器的起始编号
#define REG_HOLDING_NREGS     4+48+16	   	//Hold寄存器的数量，每个寄存器为2个字节

#define REG_INPUT_START   	1	//Input寄存器的起始编号
#define REG_INPUT_NREGS     5	   	//Input寄存器的数量，每个寄存器为2个字节

#define VER 80
#define AUTH	0


static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegInputStart = REG_INPUT_START;
//--------------modbus------------------------------

extern void getline (char idata *, char);  /* external function:  input line       */
extern void serial_init (void);            /* external function:  init serial UART */
//const char ver[]="1.1";
void init_sys(void);
void start();
void stop();
void set_y(char);
void clear_y(char);
void set_x(char);
void clear_x(char);
void wait_normal();
bit get_y(char);

void movein();
void moveout();


unsigned int counter0;         /* counter for pack       */
unsigned int counter_y19=0;
unsigned int test1,test2;
unsigned char t15;
unsigned char t_dy21;


data  unsigned char in[8];
data  unsigned char out[8];
xdata unsigned char delay_t[48];
const unsigned char parament_default[48]=
{5,//T0,100mS
	3,//T1,100mS
	30,//T2,100mS
	3,//T3,100mS
	6,//T4,1S
	3,//T5,100mS
	3,//T6,100mS
	40,//T7,100mS
	//--------------
	8,//T8,100mS
	40,//T9,100mS
	20,//T10,100mS
	4,//T11,1S
	5,//T12,100mS
	5,//T13,100mS
	5,//T14,100mS
	1,//T15,100mS
	//-------------
	2,//T16,1S
	12,//T17,100mS
	1,//T18,count
	0,//T19,100mS
	0,//T20,0/1
	3,//T21,100mS
	1,//T22,count*100
	56,//T23,count
	
	100,//T24,100mS
	11,//T25,1S
	11,//T26,1S
	26,//T27,100mS
	10,//T28,No used
	0,//T29,No used
	0,//T30,0/1 ,decide which banlence work when locked each other.
	0,//T31,0/1 ,v71 or v73 ?
	
	0,//T32,No used
	0,//T33,No used
	0,//T34,No used
	0,//T35,No used
	0,//T36,No used
	0,//T37,No used
	0,//T38,No used
	0,//T39,No used
	
	0,//T40,No used
	0,//T41,No used
	0,//T42,No used
	0,//T43,No used
	0,//T44,No used
	VER,//T45,version
	0,//T46,Debug
	0xaa//T47,No used
};
//time for 100mS count

unsigned char task_status[16];
bit status;
bit f_save;
bit f_count_change;
//bit x41,x12,x13,x22,x26;
//bit f_y25;
//bit f_get_error;

bit f_e11;
bit f_e12;
bit f_e13;
bit f_e14;
bit f_e16;
bit f_e17;
bit f11,f12,f13,f14;
bit f_jd;
bit f_ey2;
bit f_tc;
bit y21;
bit f_x10;
bit f_x10_1;
//bit f_x15;
bit f_first;
bit f_stop_ban=0;
bit f_run;
bit f_counter0=1;
unsigned char t_y31=0;
unsigned char	error;
unsigned char time_y9;

unsigned char control_count=0;

/******************************************************************************/
/*       Task 0 'job0':  RTX-51 tiny starts execution with task 0             */
/******************************************************************************/
void init () _task_ INIT  {    

		init_sys();
	
		
	  os_create_task (SEW_PACK);               
		os_wait2(K_TMO,10);
	
		os_create_task(STOP_BAN);
		os_wait2(K_TMO,10);

		//count
		os_create_task(Y19_COUNT);
		//count

		os_create_task	(COMM);
		os_wait2(K_TMO,10);	
	
		os_create_task(MODBUS);
		os_wait2(K_TMO,10);

		os_create_task(TIME_100MS);
		os_wait2(K_TMO,10);
		
		wait_normal();
		if(AUTH)					while(control_count==0)os_wait2(K_TMO,100);
		
		os_create_task (SYS_CON);               
		os_wait2(K_TMO,250);	
		
		
		os_create_task (MAIN_TASK);               
		os_wait2(K_TMO,10);
		
		
		
		os_delete_task(INIT);
  
}
void init_sys()
{
	char i;
	unsigned long int j;
	serial_init();
	error=0;
	f_count_change=0;
	f_e11=0;
	f_e12=0;
	f_e13=0;
	f_e14=0;
	f_e16=0;
	f_e17=0;
//	f_x15=1;
	test1=0;
	test2=0;
	f_jd=0;
	f_ey2=1;
	f_tc=0;
	time_y9=0;
	y21=0;
	t_dy21=0;
	f_first=1;
	
		
	os_wait2(K_TMO,100);
	os_wait2(K_TMO,100);
	os_wait2(K_TMO,100);
	os_wait2(K_TMO,100);
//	os_wait2(K_TMO,100);
	for(j=0;j<2;j++)
	{
		for(i=0;i<8;i++)out[i]=0x00;
		for(i=0;i<16;i++)task_status[i]=0x00;
	}
	
	set_y(2);
	clear_y(1);
	moveout();
	status=STOP;
	
	for(i=0;i<48;i++)
	{
		delay_t[i]=Iap_Read(i);
	}

	
	counter0=((unsigned int)Iap_Read(0x200)<<8)+Iap_Read(0x201);
	counter_y19=((unsigned int)Iap_Read(0x202)<<8)+Iap_Read(0x203);
	
//	if(delay_t[47]!=0xaa)
	//	for(i=0;i<48;i++)		delay_t[i]=parament_default[i];

	
}

void wait_normal()
{
	//	unsigned char i;
	//	static bit pr_x9=0,x9;
	
		os_create_task (FLASH_LED);               
		os_wait2(K_TMO,10);
		
		set_y(5);
		set_y(6);
		set_y(7);
		clear_y(15);
	
		os_create_task(CHECK_X14);
		set_y(18);
		os_wait2(K_TMO,delay_t[13]*10);
		clear_y(18);
	
		while(f_e14)	os_wait2(K_TMO,2);
		
	
		set_y(16);
	
		t15=delay_t[17]*10;
									
		//13-----------------------
		task_status[MAIN_TASK]=93;
		if(!X7)
		
	
		{
			//91-----------------------
			task_status[MAIN_TASK]=91;
			/*
			if(X10)
			while(1)
			{
				x9=X9;
				if(x9&&(!pr_x9)){set_y(3);}
				os_wait2(K_TMO,2);
				//if((X10)&&(!X20))break;
				if((get_y(3))&&(!X10)){clear_y(3);break;}
				pr_x9=x9;
			}
			*/
		
		}
		f_x10=X10;
		f_x10_1=X10;
		f_ey2=0;
		set_y(2);

}
/******************************************************************************/
void sys_con() _task_ SYS_CON  {
	static bit pre_key_run=1,pre_key_stop=1,pre_set=0;
	static bit cls_x9=1;

	
//	printf("task SYS_CON started!\n");
/*
	
	eMBErrorCode    eStatus;
	
	f_save=0;
  eStatus = eMBInit( MB_RTU, 0x0A, 2, 19200, MB_PAR_NONE ); 
  eStatus = eMBEnable(  );
*/
	os_wait2(K_TMO,100);

	while(Y63==0)
 	{
		movein();
		moveout();
		/*
		//modbus-------
		eMBPoll(  );
		
		if(f_save)
		{
			Iap_Erase(0);
			for(i=0;i<48;i++)
			{
				Iap_Write((unsigned int)i,delay_t[i]);
			}
			f_save=0;
		}
		//modbus-------
		*/
		if(X8&&(!pre_key_stop))f_save=1;
		if(!X9)cls_x9=1;
		if(Y63==0)
		{
			if(X17)
			{
				if(X18&&(!pre_key_run)&&(!X8)&&(!f_ey2))
					start();
				else if(X8&&(!pre_key_stop)&&(!X18)&&(f_jd))
					stop();
				clear_y(0);
			}
			else 
				{
					os_wait2(K_TMO,10);
					if(!X17)
					if((error==0)||(error>=100))error=17;
					set_y(0);stop();
				}
			
			if(status==STOP)
			{
				if(X6)
				{
					os_wait2(K_TMO,20);
					if(X6)
					set_y(4);
				}
				else clear_y(4);
			}
	//3------------------------------------
	
			if(X7)
			{
				if((!X5)&&(!X1)&&(f_jd)&&(cls_x9)&&(X9)&&(!get_y(3)))	{set_y(3);cls_x9=0;}
			}
			else
			{
				if((f_jd)&&(cls_x9)&&(X9)&&(!get_y(3))&&(!X20))	{set_y(3);cls_x9=0;}
				if((X20)&&(get_y(3)))
				{
					if(!get_y(20)){clear_y(3);}
					else 
					{
							if(delay_t[30]==1)
							{
								clear_y(20);
								clear_y(3);
							}
							else
							{
								//do nothing.
							}
					}
				}
			}
			if(X5){os_delete_task(Y8_ON);clear_y(8);}
			
			pre_key_run=X18;
			pre_key_stop=X8;
			pre_set=X6;
			
		}
		os_wait2(K_TMO,2);
		delay_t[46]=task_status[MAIN_TASK];
 	}
}

//---------------ok----------------------
void flash_led() _task_	FLASH_LED 
{
	
//	printf("task FLASH_LED started!\n");
	while(1)
	{
		movein();
		moveout();
		if(Y63==0)
		{//--------------------	
			if(f_e11)	set_y(11);
			if(f_e12)	set_y(12);
			
			if(f_e13)	set_y(13);
			if(f_e14)	set_y(14);
			if(f_ey2) set_y(2);
			else {clear_y(2);os_wait2(K_TMO,20);}//if delete this,y2 will not flash when stop banace.
	//		if(f_e15)	set_y(15);
	//		if(f_e16)	set_y(16);
	//		if(f_e17)	set_y(17);
			
			os_wait2(K_TMO,50);
			clear_y(11);
			clear_y(12);
			clear_y(13);
			clear_y(14);
			if(f_ey2)clear_y(2);
	//		clear_y(15);
	//		clear_y(16);
		//	clear_y(17);
			
			os_wait2(K_TMO,50);
		
		}
		else os_wait2(K_TMO,2);
	}
}
//------------------------------------------
void main_task()	_task_	MAIN_TASK
{
	unsigned char i;
//	unsigned int temp_i;
	bit f_9_10=0;
	
//	printf("task MAIN_TASK started!\n");
	
	while(1)
	{
		movein();
		moveout();
		if(Y63==0)
		{
			if(status==RUNING)
			{
				//wait for X9
				//3-----------------------
				//task_status[MAIN_TASK]=3;
				//if(X9)	set_y(3);
				//4-----------------------
				f_run=1;
				if(f_first){t_y31=delay_t[0];f_first=0;}
				task_status[MAIN_TASK]=3;
				if(X7)	
				{
					while(!get_y(3)){os_wait2(K_TMO,2);if(error==0)error=203;}
					//add by v13
					if(error==203)error=0;
					while(!X23)
					{
						os_wait2(K_TMO,2);
						if(error==0)error=123;
					}
					if(error==123)error=0;	
					//add by v13
				}
				else
				{
					task_status[MAIN_TASK]=4;
					
					while(1)
					{
						os_wait2(K_TMO,2);
						//if((X16)&&(!X0)&&(!get_y(19)))break;
						if((X23)&&(X16)&&(!X0)&&(!get_y(19)))break;
						if(!X23)if(error==0)error=123;
						if(!X16)if(error==0)error=116;
						if(X0)if(error==0)error=190;
						if(get_y(19))if(error==0)error=219;
						//edit by v11
					}
					if((error==123)||(error==116)||(error==190)||(error==219))error=0;
				//	if(f_x15)f_x15=0;
				//	else {if((error==0)||(error>=100))error=15;stop();}
				}
			/*	
				//add at v34
				while(X1)
					{
						os_wait2(K_TMO,2);
					}*/
				
				if(!f_x10)
				{
				set_y(10);
				
				clear_y(5);
				clear_y(6);
				clear_y(7);
				
				os_create_task(STEP_4);
				//5-----------------------
				task_status[MAIN_TASK]=5;
				os_create_task(Y8_ON);
				os_create_task(Y16_ON);
			
				//7-----------------------
				task_status[MAIN_TASK]=7;
				for(i=0;i<delay_t[2]; i++)
				{
					os_wait2(K_TMO,10);
					if(X3){break;}
					if(error==0)error=103;
					if(delay_t[20]&&(i!=0))i--;
				}
				if(error==103)error=0;
				if(X13)
				{
				  	set_y(16);
					//add by v14
				}
				test1=delay_t[2];
				test2=i;
				if(i<delay_t[2])
				{
					//6-----------------------
					task_status[MAIN_TASK]=6;
					if(X3)
					{	
						f12=0;
						for(i=0;i<delay_t[1]*5;i++)
						{
							os_wait2(K_TMO,2);
							if(X12)
							{
								f12=1;
								//set_y(16); //delete by v16
								//add by v11	
							}
						}
						set_y(5);
						if(!f12){f_e12=1;if((error==0)||(error>=100))error=12;stop();}
					}
					
					//8-----------------------
					task_status[MAIN_TASK]=8;
					//wait for X4
					while(!X4)
					{
						os_wait2(K_TMO,2);if(error==0)error=104;
					}
					if(error==104)error=0;
					f13=0;
					for(i=0;i<delay_t[5]*5;i++)
					{
						os_wait2(K_TMO,2);
						if(X13)
						{
							f13=1;
						  //	set_y(16);
							//add by v14
						}
						
					}
					set_y(6);
					if(!f13){f_e13=1;if((error==0)||(error>=100))error=13;stop();}	
					
				}
				else
				{
					set_y(15);
					set_y(5);
					set_y(6);
					os_wait2(K_TMO,delay_t[3]*10);
					clear_y(15);
					//set_y(9);
					//for(i=0;i<delay_t[4];i++)
					//os_wait2(K_TMO,100);
					//clear_y(9);
					
					time_y9=delay_t[4]*10;
				}
			}
			else
			{
				f_x10=0;
				if(error==0) error=110;
				if(!X1)
				{
					set_y(8);
					set_y(28);
					for(i=0;i<delay_t[21];i++)os_wait2(K_TMO,10);
					clear_y(28);
					f_9_10=0;
				}
				else f_9_10=1;
			}				
			
			if(!f_9_10)
			{
				//9-----------------------
				
				task_status[MAIN_TASK]=9;
				//wait for X5
				while(!X5)
				{
					os_wait2(K_TMO,2);
					if(error==0)error=105;
				}
				if((error==105)||(error==110))error=0;
				os_delete_task(Y8_ON);
				clear_y(8);
				
				f14=0;
				for(i=0;i<delay_t[6];i++)
				{
					os_wait2(K_TMO,10);
					if(X14)
					{
						f14=1;
						t15=delay_t[15]*10;//add by v15
						//os_create_task(Y16_OFF);
						//add by v11
					}
				}
				set_y(7);
				
				moveout();
				movein();
				if(!f14){f_e14=1;if((error==0)||(error>=100))error=14;stop();}
		
				
				//10-----------------------
				task_status[MAIN_TASK]=10;
				movein();
				if((X5)&&(!X20)){set_y(20);moveout();}
				else if((X20)&&(!X7))
				{
					clear_y(3);
					while(X20){os_wait2(K_TMO,2);if(error==0)error=120;}
					if(error==120)error=0;
					set_y(20);moveout();
				}
				
			}
			else f_9_10=0;
												
				//13-----------------------
				task_status[MAIN_TASK]=13;
				if(X7)
				{
					while(!X1)		os_wait2(K_TMO,2);
					
					if(X20)
					{						
						while(X20)		{os_wait2(K_TMO,2);if(error==0)error=120;}
						
						for(i=delay_t[10];i>0;i--)						os_wait2(K_TMO,100);
						//os_wait2(K_TMO,delay_t[10]*10);
					}	
					else	//os_wait2(K_TMO,delay_t[14]*10);
					{
						for(i=0;i<delay_t[14]/10;i++)
						os_wait2(K_TMO,100);
						os_wait2(K_TMO,((delay_t[14])%10)*10);
					}
					if(error==120)error=0;		
					
					clear_y(3);
					clear_y(20);
					y21=1;
					clear_y(19);
					
					
		//			os_create_task(STEP_13);
							
				}
				else
				{
					//11-----------------------
					task_status[MAIN_TASK]=11;
		//			if((X5)&&(!X20)){set_y(20);moveout();}
					while(1)
						{
							if((X5)&&(!X20)) {if(error==120)error=0;break;}
							os_wait2(K_TMO,2);
							if(error==0)error=120;
						}
					{set_y(20);moveout();}
					
					while(1)
					{
						os_wait2(K_TMO,2);
						if(get_y(3)&&(X1)&&(!X20)&&(!X28))break;
						if(get_y(3)&&(X1)&&(!X20)&&(X31))break;
						if(!get_y(3)){if(error==0)error=203;}else if(error==203)error=0;
						if(!X1){if(error==0)error=101;}else if(error==101)error=0;
						if(X20){if(error==0)error=120;}else if(error==120)error=0;
						if(!X31){if(error==0)error=131;}else if(error==131)error=0;
					}
					set_y(19);
					if((error==203)||(error==101)||(error==120)||(error==131))error=0;
					
					
					//12-----------------------
					task_status[MAIN_TASK]=12;
					//wait for !X1
					while(X1||!X15)
					{
						os_wait2(K_TMO,2);
						if(X1){if(error==0)error=191;}else if(error==191) error=0; 
						if(!X15)if(error==0)error=115;
						
					}
					
					clear_y(19);
					if((error==191)||(error==115))error=0;
					os_create_task(STEP_12);
					moveout();
				}
				
				//
			}
			else 
			{
				
				f_run=0;
				f_jd=0;
				f_ey2=0;
				set_y(2);
				os_delete_task(Y8_ON);
				clear_y(8);
				set_y(5);
				set_y(6);
				set_y(7);
				task_status[MAIN_TASK]=1;
			}
				
		}
		os_wait2(K_TMO,2);
	}
}
void time_100ms() _task_ TIME_100MS
{

	static bit x24=0,x25=0,x26=0,x27=0;
	static bit x29=0,x30=0;
	static bit pre_x29=0,pre_x30=0;
	static bit pre_y21=0;
	static bit pre_x24=0,pre_x25=0,pre_x26=0,pre_x27=0;
	static bit pre_x3=0,x3=0;
	static bit pre_y3=0,y3=0;
	
	static bit triged=0;
	static bit pre_x15=0,x15=0;
	static bit pre_x1=0,x1=0;
//	static bit d21=0;
	static unsigned char t_run1=0,t_run2=0;
	static unsigned char t_y26off=0,t_y2627on=0,t_y27off=0;
	static unsigned char t_y3=0;
	static unsigned char t_y29=0,t_y30=0;
	static unsigned int 	auto_trig=0;
	
	
	while(Y63==0)
	{
	//X21/X24/X25
		

		if(delay_t[7]==0)delay_t[7]=1;
		if(delay_t[9]==0)delay_t[9]=1;
		
		x24=X24;
		x25=X25;
		x26=X26;
		x27=X27;
		
		x29=X29;
		x30=X30;
		
		x15=X15;
		x1=X1;
		
	//	if(x15&&!pre_x15)f_x15=1;
	//	pre_x15=x15;
		
		
		x3=X3;
		y3=get_y(3);
	//..	proces herer
	//	if((y21)&&(!pre_y21))	d21=1;
		if((x24)&&(!pre_x24))	
		{
				t_dy21=0;
				clear_y(21);
				clear_y(24);
			/*
				if(x25&&(status==RUNING))clear_y(24);
				else
				{
					if(t_run1)
					{t_run2=delay_t[24];set_y(25);}
				}
			*/
				t_run1=0;
		}
		if((x25)&&(!pre_x25))	{t_run2=0;if(!get_y(21))clear_y(24);clear_y(25);}
		
	//	if((pre_x25)&&(!x25)&&(x24)&&(status==RUNING))	{t_run2=delay_t[24];set_y(24);set_y(25);}
			
		if((x27)&&(!pre_x27))	y21=1;
		
		if((pre_x24)&&(!x24)&&(y21)&&(delay_t[19])&&(status==RUNING))	
		{t_run1=delay_t[24];set_y(24);set_y(21);y21=0;t_y27off=delay_t[9];clear_y(26);}	
		
		if((x26)&&(!pre_x26))	
		{
			
				/*if(get_y(21)||get_y(24)||get_y(25))
				{
					t_dy21=0;
					clear_y(21);
					clear_y(24);
					clear_y(25);
				}
				else
			*/
				if(!(get_y(21)||get_y(24)||get_y(25)))//start run
				{
				if((x24)&&(!x25))
					{	
						t_run2=delay_t[24];set_y(24);set_y(25);
						if(y21){t_dy21=delay_t[27];t_run1=delay_t[24];/*set_y(21);*/
						y21=0;t_y27off=delay_t[9];clear_y(26);}
					}
				if((!x24)&&(y21))
					{t_run1=delay_t[24];set_y(24);set_y(21);y21=0;t_y27off=delay_t[9];clear_y(26);}
				}
		}
		if(X7)
		{
			if((x15)&&(!pre_x15))	
			{
				if(get_y(21)||get_y(24)||get_y(25))
					{
						t_dy21=0;
						clear_y(21);
						clear_y(24);
						clear_y(25);
					}
			}
		}
//v25 v26 added @20151222		
		
		if((x3)&&(!pre_x3)&&f_run)	{set_y(26);t_y26off=delay_t[7];f_stop_ban=0;}//v73
		if(t_y26off){t_y26off--;}
		if(delay_t[31]==0)
		{
			if(t_y26off==1){clear_y(26);}
			if(((x1)&&(!pre_x1)&&f_run)||f_stop_ban)	{set_y(26);set_y(27);f_stop_ban=0;}//v73
		}
		else
		{
			if(t_y26off==1){clear_y(26);t_y2627on=delay_t[7];}
			if(t_y2627on)t_y2627on--;
			if(t_y2627on==1){set_y(26);set_y(27);};
		}
		
		if(t_y27off)t_y27off--;
		if(t_y27off==1){clear_y(27);};
		
//auto scroll
		if((!y3)&&(pre_y3))
		{
			t_y3=delay_t[19];
		}
		
		if(t_y3==1)
		{
			if((x24)&&(!x25)&&(!t_run1))
				{	
					t_run2=delay_t[24];set_y(24);set_y(25);
					if(y21){t_dy21=delay_t[27];t_run1=delay_t[24];/*set_y(21);*/y21=0;t_y27off=delay_t[9];clear_y(26);}
				}
			else if((!x24)&&(y21)&&(!t_run2))
				{t_run1=delay_t[24];set_y(24);set_y(21);y21=0;t_y27off=delay_t[9];clear_y(26);}
		}	
		if(t_y3)t_y3--;
		pre_y3=y3;
//end autoscroll
		

		
		pre_x3=x3;
		pre_x1=x1;

//v25 v26 added @20151222	
		
		//for debug only
		/*
		if(y21)set_y(31);
		else clear_y(31);
		if(t_run1)set_y(32);
		else clear_y(32);
		if(t_run2)set_y(33);
		else clear_y(33);
		*/
		//end debug-------
		
	//..	roces herer end
		pre_y21=y21;
	
		pre_x24=x24;
		pre_x25=x25;
		pre_x26=x26;
		pre_x27=x27;
		pre_x15=x15;
		
		
	//---end------X21/X24/X25	
		
		if(time_y9){time_y9--;set_y(9);}
		else clear_y(9);
		//--------------------
		
		if(t15){t15--;}
		if(t15==1)clear_y(16);
		//--------------------

		
//		if(t_24){t_24--;}
		if(t_run1){t_run1--;}
		if(t_run1==1){clear_y(24);clear_y(21);t_dy21=0;}
		if(t_run2){t_run2--;}
		if(t_run2==1) {clear_y(25);clear_y(24);}
		if(t_y31)
		{
			t_y31--;set_y(31);
		}
		else clear_y(31);
		
		if(t_dy21==1) set_y(21);
		if(t_dy21)t_dy21--;
		//--------------------
		if(status==RUNING)
		{
			if((delay_t[28])&&(++auto_trig>=delay_t[28]*600*60))
			{
				auto_trig=0;
				if((!t_y29)&&(!t_y30))
				{
					triged=1;
					{set_y(29);clear_y(30);t_y30=0;t_y29=delay_t[25]*10;};
				}
			}
		}
		
		
		if(!triged)
		{
			if(x29&&(!pre_x29)){set_y(29);clear_y(30);t_y30=0;t_y29=delay_t[25]*10;};
			if(x30&&(!pre_x30)){set_y(30);clear_y(29);t_y29=0;t_y30=delay_t[26]*10;};
		}		
		if(t_y29){t_y29--;}
		if(t_y29==0)
		{
			clear_y(29);
			if(triged)
			{
				set_y(30);
				clear_y(29);
				t_y29=0;
				t_y30=delay_t[26]*10;
				triged=0;
			}		
		}
		if(t_y30){t_y30--;}
		if(t_y30==0)clear_y(30);
		
		
		pre_x29=x29;
		pre_x30=x30;
		
		os_wait2(K_TMO,10);
	}
}
void stop_ban() _task_ STOP_BAN
{
	//
	static bit x16=0,pre_x16=0;
	static bit x19=0,pre_x19=0;
	while(1)
	{
		x19=X19;
		if(((x19)&&(!pre_x19)&&(!get_y(10)))&&(status==RUNING))
		{
			while(X5)os_wait2(K_TMO,10);
			os_delete_task(MAIN_TASK);
			f_tc=1;
			set_y(18);
			set_y(5);
			set_y(6);
			set_y(7);
			clear_y(9);
			os_delete_task(Y8_ON);
			clear_y(8);
			clear_y(15);
			clear_y(3);
			
			os_wait2(K_TMO,delay_t[13]*10);
			clear_y(18);
				
			if((error==0)||(error>=100))error=90;
			stop();
			f_stop_ban=1;
			f_x10=X10;
			f_x10_1=X10;
		
			os_create_task(MAIN_TASK);
		}
		pre_x19=x19;
		//-----------------------------
		x16=X16;
		
		if(((!x16)&&(pre_x16)&&(!get_y(19)))&&(status==RUNING))
		{
			while(X5)os_wait2(K_TMO,10);
			os_delete_task(MAIN_TASK);
			f_tc=1;
			set_y(18);
			set_y(5);
			set_y(6);
			set_y(7);
			clear_y(9);
			os_delete_task(Y8_ON);
			clear_y(8);
			clear_y(15);
			clear_y(3);
			
			os_wait2(K_TMO,delay_t[13]*10);
			clear_y(18);
				
			if((error==0)||(error>=100))error=16;
			stop();
		
			f_x10=X10;
			f_x10_1=X10;
		
			os_create_task(MAIN_TASK);
		}
		
		pre_x16=x16;
		//--------------------------------
		os_wait2(K_TMO,5);
	}
}
void step_12() _task_ STEP_12
{
	unsigned char i;
	for(i=0;i<delay_t[8]/10;i++)
	os_wait2(K_TMO,100);
	os_wait2(K_TMO,((delay_t[8])%10)*10);
	clear_y(3);
	clear_y(20);
	f_jd=0;
//	os_create_task(DISABLE_JD);
	os_wait2(K_TMO,100);
	f_jd=1;
	os_delete_task(STEP_12);
}
/*
void step_13() _task_ STEP_13
{
	unsigned char i;

	//os_wait2(K_TMO,delay_t[9]*10);
//	for(i=0;i<delay_t[9];i++)
//	os_wait2(K_TMO,100);
//	while(X24){os_wait2(K_TMO,10);}
//	set_y(21);
	y21=1;
//	for(i=0;i<delay_t[10];i++)
//	os_wait2(K_TMO,100);
	
//	clear_y(21);
	clear_y(19);
	os_delete_task(STEP_13);
	
}
*/
void y19_count() _task_ Y19_COUNT
{
	static bit pr_x10;
	unsigned char i;
	bit x10;
	while(1)
	{
		x10=X15;
		if((x10)&&(!pr_x10))
		{
			counter0++;
			f_counter0=1;
			if(AUTH)
			{
				if(control_count)control_count--;
				else 
				{
					error=98;
					stop();
					printf("**-");
					os_delete_task(MAIN_TASK);
					os_wait2(K_TMO,100);
					os_delete_task(SYS_CON);
					
				}

			}
			counter_y19++;
			
			f_count_change=1;
			if(counter_y19>=(delay_t[22]*100+delay_t[23]))
			{		
				counter_y19=0;
				set_y(17);
				//os_wait2(K_TMO,delay_t[16]*10);
				for(i=0;i<delay_t[16];i++)
				os_wait2(K_TMO,100);
				clear_y(17);
			}
		}
		pr_x10=x10;
		os_wait2(K_TMO,20);
	}
}
void step_4()	_task_ STEP_4
{
	unsigned char i;
	f11=0;
	for(i=0;i<delay_t[0]*5;i++)
	{
		os_wait2(K_TMO,2);
		if(X11)f11=1;
	}
	clear_y(10);
	if(!f11){f_e11=1;if((error==0)||(error>=100))error=11;stop();}
	os_delete_task(STEP_4);
				
}
void y8_on() _task_ Y8_ON
{
	//os_wait2(K_TMO,delay_t[11]*10);
	unsigned char i;
	for(i=0;i<delay_t[11];i++)
	{
		os_wait2(K_TMO,100);
		if(X1)break;
		
	}
	if(i>=delay_t[11])	if(!f_tc)set_y(8);
	os_delete_task(Y8_ON);
}
//add by v11
/*
void y16_off() _task_ Y16_OFF
{
	unsigned char i;
	for(i=0;i<t15;i++)//v12
	os_wait2(K_TMO,100);//v12
	clear_y(16);
	os_delete_task(Y16_OFF);
}*/
//add by v11	
//add by v16
void y16_on() _task_ Y16_ON
{
	while(!X13)		os_wait2(K_TMO,2);
	set_y(16);
	os_delete_task(Y16_ON);
}
//add by v16	

//add by v16
void check_x14() _task_ CHECK_X14
{
	while(!X14)
	{
		f_e14=1;
		os_wait2(K_TMO,2);
		if(error==0)error=114;
	}
	f_e14=0;
	if(error==114)error=0;
	os_delete_task(CHECK_X14);
}
//add by v16	
void sew_pack()	_task_	SEW_PACK
{
	static bit pr_x38=0,pr_x37=0;
	unsigned char i;
	bit x37,x38;
//	unsigned char i;
//	printf("task SEW_PACK started!\n");
	
	while(1)
	{
		movein();
		moveout();
		if(Y63==0)
		{
			x37=X21;
			x38=X22;
			if(status==RUNING)
			{
				//1-------------
				task_status[SEW_PACK]=1;
					
				if((x37)||(x38))
				{
					set_y(22);
					os_wait2(K_TMO,1);
				}
				if((!x37)&&pr_x37&&(!x38))	
				{
				
					task_status[SEW_PACK]=3;
					//error=38;
					clear_y(22);
				//	stop();
				}
				
				if((!x38)&&(pr_x38))
				{
					if((!x38)&&(!x37))
					{
						clear_y(22);	
						for(i=0;i<delay_t[18];i++)
						{
							set_y(23);
							os_wait2(K_TMO,delay_t[12]*10);
							clear_y(23);
							os_wait2(K_TMO,delay_t[12]*10);
						}
					}
				}
				
			}
			else
			{
				//3-------------
			//	task_status[SEW_PACK]=6;	
			
				clear_y(22);
				clear_y(23);
							
			}
			pr_x38=x38;
			pr_x37=x37;
		}
		os_wait2(K_TMO,2);
	}
}

void comm()	_task_	COMM
{
	int i=0;
	unsigned char step=0;
	unsigned char ch;
	printf("task COMM started!\n");	
	printf("\nver---%d\n",VER&0x0ff);
	
	while(1)
	{
	/*
		printf("\r\ntest= %d",i++);
		printf("\r\ncounter0=%ld",counter0);*/
		
		ch=(_getkey ());
		switch(step)
		{			
			case 0:
				if(ch==0x06) step++;
			break;
			case 1:
				if(ch==0x00) step++;
				else if(ch==0x06) step=1;
							else step=0;
			break;
			case 2:
				if(ch==0x01) step++;
				else if(ch==0x06) step=1;else step=0;
			break;
			case 3:
				if(ch==0x00) step++;
				else if(ch==0x06) step=1;else step=0;
			break;
			case 4:
				step=0;
				control_count=ch*10;
				if(control_count)
				{
					if(f_counter0)
					{
						printf("**%c(%d)",X2?'B':'A',counter0);
						f_counter0=0;
					}
				}
				else 
				{
					error=97;
					stop();
					printf("**.");
					os_delete_task(MAIN_TASK);
					os_wait2(K_TMO,100);
					os_delete_task(SYS_CON);
					
				}
				
			break;
			default:
				step=0;
			break;
			
		}
		
		//os_wait2(K_TMO,100);
	}
}


	
void modbus()	_task_	MODBUS
{
		unsigned char i;
	eMBErrorCode    eStatus;
	
	f_save=0;
	//i=0x0a;
	//if((P31==0)&&(P30==0)) i=0x0b;
	movein();
	if(X2)i=0x0b;
	else i=0x0a;
  eStatus = eMBInit( MB_RTU, i, 2, 19200, MB_PAR_NONE ); 
  eStatus = eMBEnable(  );

//--------------- Main Loop -------------------------------------------------
	while(1)
	{
		( void )eMBPoll(  );
		os_wait2(K_TMO,2);
		if(f_save)
		{
		//	Iap_Erase(0);
			//if(f_count_change)
			//{
		//	Iap_Erase(0x200);
				Iap_Write((unsigned int)0x200,(counter0>>8)&0xff);
				Iap_Write((unsigned int)0x201,counter0&0xff);
				Iap_Write((unsigned int)0x202,(counter_y19>>8)&0xff);
				Iap_Write((unsigned int)0x203,counter_y19&0xff);
				
		//	f_count_change=0;
		//	}
			for(i=0;i<48;i++)
			{
				Iap_Write((unsigned int)i,delay_t[i]);
			}
			f_save=0;
		}
		
	}

}


void start()
{
		if(X17)
			{
				
			set_y(1);
			clear_y(2);
			os_wait2(K_TMO,3);
			status=RUNING;

			clear_y(4);
		
			if(!f_x10_1)
			{				
				f_x10=0;
				clear_y(5);
				clear_y(6);
			}	
			clear_y(7);

			error=0;
			f_e11=0;
			f_e12=0;
			f_e13=0;
			f_e14=0;
			f_e16=0;
			f_e17=0;
				
			f_jd=1;
			f_tc=0;

	
			}
}
void stop()
{
		if(status==RUNING)
		{
			//set_y(2);
			f_ey2=1;
			clear_y(1);
			t_dy21=0;
			clear_y(21);
			clear_y(24);
			clear_y(25);
			moveout();
			status=STOP;
				
			
		}
}
void set_y(char a)
{
		out[a/8]|=(0x01<<(a%8));
}
void clear_y(char a)
{
		out[a/8]&=~(0x01<<(a%8));
}

bit get_y(char a)
{
		return (out[a/8])&(0x01<<(a%8));
}
bit get_outy(char a)
{
		return (out[7-a/8])&(0x01<<(7-a%8));
}


void set_x(char a)
{
		in[7-a/8]|=(0x01<<(a%8));
}
void clear_x(char a)
{
		in[7-a/8]&=~(0x01<<(a%8));
}
void movein()
{
		char i;
		bit inbit;
		
		PL=0;
		CP=0;
		Q7=1;
		PL=1;
		inbit=Q7;
		if(inbit)			set_x(63);
		else clear_x(63);
	
		for(i=62;i>=0;i--)
		{
			CP=1;
			inbit=Q7;
			CP=0;
			if(inbit)			set_x(i);
			else clear_x(i);
			
		}	
}
	
void moveout()
{
		char i;
		STCP=0;
		STCP=0;
		for(i=0;i<64;i++)
		{
			SHCP=0;
			if(get_outy(i))DOUT=1;
			else DOUT=0;
			SHCP=1;
			_nop_();
		}
		STCP=1;
		OE=0;
}

//-----------------------------------------------------------------------------
// eMBRegHoldingCB
//-----------------------------------------------------------------------------
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart);
        switch ( eMode )
        {

        case MB_REG_READ:
            while( usNRegs > 0 )
            {
							if(iRegIndex<4)
							{
								*pucRegBuffer++ =out[iRegIndex*2] ;
								*pucRegBuffer++ =out[iRegIndex*2+1] ;
							
							}
								else if(iRegIndex<4+48)
								{
									*pucRegBuffer++ =0;
									*pucRegBuffer++ = delay_t[(iRegIndex-4)];
							
								}
								else if(iRegIndex<4+48+15)
								{
									*pucRegBuffer++ = 0;
									*pucRegBuffer++ = task_status[iRegIndex-4-48];
							
								}
								else if(iRegIndex==4+48+15)
								{
									*pucRegBuffer++ = 0;
									*pucRegBuffer++ = error;
							
								}
								
								iRegIndex++;
                usNRegs--;
            }
            break;

        case MB_REG_WRITE:
						if(status==STOP)
						{
							while( usNRegs > 0 )
							{
															
								if(iRegIndex<4)
								{
										out[iRegIndex*2]=*pucRegBuffer++;
										out[iRegIndex*2+1]=*pucRegBuffer++;								
								}
									else if(iRegIndex<4+48)
									{
										pucRegBuffer++ ;
										delay_t[iRegIndex-4]=*pucRegBuffer++ ;
										if(iRegIndex==4+44)
											counter0=0;
										if(iRegIndex==4+45)
											delay_t[45]=VER;
										//f_save=1;				
									}
														
									iRegIndex++;
									usNRegs--;
							}
						}
						else eStatus =MB_EINVAL;
						
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart);
        while( usNRegs > 0 )
        {
						if(iRegIndex==4)
						{
						*pucRegBuffer++ = (counter0>>8)&0xff;
            *pucRegBuffer++ = counter0&0xff ;
            	
						}
						else
						{
            *pucRegBuffer++ = in[iRegIndex*2];
            *pucRegBuffer++ = in[iRegIndex*2+1] ;
            }
						iRegIndex++;
            usNRegs--;
						
        }
    }
    else
    {

			eStatus = MB_ENOREG;
	
    }

    return eStatus;
}


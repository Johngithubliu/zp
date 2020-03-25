/******************************************************************************/
/*                                                                            */
/*                     xi'an ziping pack machine factory                     */
/*                                                                           */
/******************************************************************************/
#include <intrins.h>
#include "stc12cxx.h"

#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */
#include "mydefine.h"
#include <stdio.h>
#include "24lc16.h"

//------------modbus------------------------------
#include "mb.h"
#define REG_HOLDING_START   	1	//Hold寄存器的起始编号
#define REG_HOLDING_NREGS     4+64+16	   	//Hold寄存器的数量，每个寄存器为2个字节

#define REG_INPUT_START   	1	//Input寄存器的起始编号
#define REG_INPUT_NREGS     4+4	   	//Input寄存器的数量，每个寄存器为2个字节,4*2=8byte in
																		//2*2=counter0 
																		//2*2,reside.

#define AUTH	0
#define AFTER_X48	0


//#define middle_store	X58
#define middle_store	delay_t[51]
#define ERROR_CHECK_TIME	300	
//for check active is ok?

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
bit get_y(char);
void check_counter0();

void movein();
void moveout();


unsigned long int counter0;         /* counter for pack       */

const unsigned char VER=243;

data  unsigned char in[8];
data  unsigned char out[8];
xdata unsigned char delay_t[64];
const unsigned char parament_default[64]=
{10,//T1,100mS
	5,//T2,100mS
	3,//T3,100mS
	3,//T4,100mS
	5,//T5,100mS
	10,//T6,100mS
	3,//T7,100mS
	40,//T8,10mS
	
	1,//T9,100mS
	0,//T10,100mS
	35,//T11,100mS
	0,//T12,100mS
	3,//T13,100mS
	4,//T14,100mS
	20,//T15,count
	15,//T16,100mS
	
	
	15,//T17,100mS
	26,//T18,100mS
	1,//T19,count
	5,//T20,100mS
	1,//T21,10mS
	0,//T22,10mS
	100,//T23,100mS
	1,//T24,100mS
	
	4,//T25,100mS
	4,//T26,100mS
	7,//T27,100mS
	3,//T28,100mS
	10,//T29,100mS
	10,//T30,100mS
	15,//T31,100mS
	15,//T32,100mS
	
	3,//T33,10mS
	15,//T34,100mS
	20,//T35,100mS
	1,//T36,count
	0,//T37,10mS
	0,//T38,100mS
	20,//T39,100mS
	10,//T40,100mS
	
	1,//T41,100mS
	0,//T42,100mS
	3,//T43,10mS
	20,//T44,100mS
	20,//T45,100mS
	5,//T46,100mS
	2,//T47,10mS
	0,//T48
	0,//T49
	20,//T50,200mS
	20,//T51,200mS
	0,//T52
	0,//T53
	0,//T54
	0,//T55
	0,//T56
	0,//T57
	0,//T58
	0,//T59
	0,//T60
	0,//T61
	0,//T62
	0,//T63
		
	0xaa //T64,Flag
};
xdata unsigned char task_status[16];
bit status;
bit f_save;
bit x41,x12,x13,x22,x26;
bit f_y25;
bit f_get_error;
bit f_open;
bit xstop;
bit f_first;
bit f_back_enable;
bit f_back_enable_x18;
bit f_x5;
bit f_x3;
bit f_x42;
bit f_y41;
bit f_x2_enable;
bit first_push=0;

//bit x35sx48;
bit shadow_y23=0;
bit shadow_y49=0;
bit x48_check=0;
bit x49_check=0;
bit f_counter0=1;

unsigned int t_disable_y2=0;						  
unsigned char	error;
unsigned char t_24,t_36,t_3,t_8,t_y5,t_y15;
unsigned int t_35,t_37;
unsigned char t_y42_off,t_y43_off,t_y44_off;
unsigned char t_y41on=0;
unsigned char s_delay_13,s_delay_21;
unsigned int t9;
unsigned int t29=0,t30=0;

unsigned int t_y47off=0;
unsigned int 	t_y53off=0,t_y52off=0;
unsigned char count_y41=0;
unsigned int t_x48_check=0;
unsigned int t_x49_check=0;

unsigned char control_count=0; 
/******************************************************************************/
/*       Task 0 'job0':  RTX-51 tiny starts execution with task 0             */
/******************************************************************************/
void init () _task_ INIT  {    
	
		init_sys();
	
	
		os_create_task(OPEN_BAG);
		os_wait2(K_TMO,10);
		os_create_task(GIVE_BAG);               
		os_wait2(K_TMO,10);
		os_create_task(GET_BAG);                
		os_wait2(K_TMO,10);
		os_create_task(ARRANGE_BAG);
		os_wait2(K_TMO,10);
	
	  os_create_task(UP_BAG);               
    os_wait2(K_TMO,10);
	  os_create_task(PUSH_PACK);
		os_wait2(K_TMO,10);	
	  os_create_task(SEW_PACK);               
		os_wait2(K_TMO,10);

		os_create_task(COMM);
		os_wait2(K_TMO,10);	
		os_create_task(MODBUS);
		os_wait2(K_TMO,10);
		
		if(AUTH)					
			while(control_count==0)
			{
				os_wait2(K_TMO,100);
				error=97;
			}
		error=0;
		
		os_create_task (SYS_CON);               
		os_wait2(K_TMO,250);	
	
		os_delete_task(INIT);
  
}
void init_sys()
{
	char i;
	unsigned long int j;
	serial_init();
	error=0;
	t_24=0;
	t_35=0;
	t_36=0;
	t_37=0;
	t_3=0;
	t_8=0;
	t_y5=0;
	t_y15=0;
	f_x5=0;
	f_x3=0;
	f_y41=0;
	t9=0;
	
	t_y43_off=0;
	t_y44_off=0;
	
	x12=0;
	x13=0;
	x22=0;
	f_y25=0;
	f_get_error=0;
	f_open=0;
	
	f_first=1;
	f_back_enable=0;
	f_back_enable_x18=0;
	xstop=0;

	//set_y(46);
	moveout();
	//for(j=0;j<200000;j++)
	
	os_wait2(K_TMO,100);
	os_wait2(K_TMO,100);
	os_wait2(K_TMO,100);
	os_wait2(K_TMO,100);
	os_wait2(K_TMO,100);
	for(j=0;j<2;j++)
	{
		for(i=0;i<8;i++)out[i]=0x00;
		for(i=0;i<16;i++)task_status[i]=0x00;
	}
	set_y(4);

	
	clear_y(1);
	moveout();
	status=STOP;
	
	for(i=0;i<64;i++)
	{
		delay_t[i]=Iap_Read(i);
	}
	movein();
	counter0=((unsigned long int)Iap_Read(0x200)<<24)+((unsigned long int)Iap_Read(0x201)<<16)+((unsigned long int)Iap_Read(0x202)<<8)+(unsigned long int)Iap_Read(0x203);
	if(X26&&X27&&X43)counter0=0l;//for clear counter0 value.
	//if(X7)counter0=0x01fffffe;
	
	//delay_t[46]=VER;
	//if(delay_t[47]!=0xaa)
	//	for(i=0;i<48;i++)		delay_t[i]=parament_default[i];
	set_y(46);
}

/******************************************************************************/
void sys_con() _task_ SYS_CON  {
	static bit pre_key_run=1,pre_key_stop=1;//pre_x41=0;
	static bit x1,x4,pre_x1=0,pre_x4=0;
	static bit pre_x3=0,pre_x19=0,pre_x18=0,pre_x37;
	static bit pre_x44=0;
	static bit x18,x37;
	static bit pre_x45=0,x45,ppre_x45=0;
	static bit pre_x34=0,x34,ppre_x34=0;
	static bit pre_x35=0,x35;
	static bit pre_x48=0,x48,ppre_x48=0;
	static bit pre_x49=0,x49;
	static bit pre_x33=0,x33;
//	static bit pre_x2=0,x2=0;
	static bit pre_x1835,x1835=0;
	static bit pre_x20=0,x20;
	
static bit pre_x0=1,x0;
static bit pre_x50=1,x50;
static bit pre_x51=1,x51;
static bit pre_x52=1,x52;
static bit pre_x53=0,x53;
static bit pre_x54=0,x54;

	static bit pre_x55=0,x55,ppre_x55=0;
//	static bit pre_x56=0,x56;
	static bit pre_x57=0,x57,ppre_x57=0;
	static bit pre_y23=0,y23;
	static bit pre_x46=1,x46,ppre_x46=1;
	static bit pre_x26=0,x26;
	static bit pre_x27=0,x27;
	
//	static unsigned t_y47=0;
	static unsigned char t_y40=0,t_y5=0,t_ystop=0;
	static unsigned int t_x18=0;
//	static unsigned char t_y20=0;
	static unsigned char t02s=0,t05s=0,t1s=0;
	static unsigned char t2d5s=0;
	static unsigned int 	t_y11_on=0;
	static unsigned int t_y48_on=0;
	static unsigned int t_y48_off=0;
	
	static unsigned char status_updown=0;
	
//	printf("task SYS_CON started!\n");
	os_wait2(K_TMO,100);
	//x35sx48=delay_t[62]&0x01;
	//if(x35sx48)set_y(60);
	//else clear_y(60);
	/*if((X55)!=(delay_t[62]&0x02)){error=55;stop();}
	if((X56)!=(delay_t[62]&0x04)){error=56;stop();}
	if((X57)!=(delay_t[62]&0x08)){error=57;stop();}
	*/
	while(1)
 	{
		movein();
		moveout();
		os_wait2(K_TMO,1);
		
		if(Y63==0)
		{
			x0=X0;
			x1=X1;
			x4=X4;
			//move X34 code to here
			if(x4&&!pre_x4)
			{
				//delay_t[62]=0;
				if(X56)delay_t[62]|=0x01;
				if(get_y(48))delay_t[62]&=~0x01;
				
			
				f_save=1;
				
				error=198;
				stop();
				while(X4) os_wait2(K_TMO,200);
			}
			
			//end X34
			
			if(x0&&pre_x0)
			{
				if(x1&&pre_x1&&(!pre_key_run)&&(!x4)&&!pre_x4)
					start();
				else if(x4&&pre_x4&&(!pre_key_stop)&&(!x1)&&!pre_x1)
				{	
					error=198;
					stop();
				}
					
			}
			else if((!x0)&&(!pre_x0)){if((error==0)||(error>100))error=99;stop();}
  

			if(x1&&pre_x1)		pre_key_run=1;
			if(!x1&&!pre_x1)	pre_key_run=0;
			if(x4&&pre_x4)		pre_key_stop=1;
			if(!x4&&!pre_x4)	pre_key_stop=0;
			
			pre_x0=x0;
			pre_x1=x1;
			pre_x4=x4;
		
		//	os_wait2(K_TMO,1);
			//---------y5
			if(X14){;clear_y(19);}
		
			//--------y6
			if((error==0)||(error>=100))clear_y(6);
			else if(error<100)set_y(6);
			
		

//--------------------------	
		if(t_35)t_35--;

		if(t_36)t_36--;
		if(t_37)t_37--;

		if(t_8)t_8--;
		
		if(t_3){set_y(3);t_3--;}
			else clear_y(3);

		if(t_24){set_y(24);t_24--;}
			else clear_y(24);
	
		
		
		if((X20)&&(X21)){t_35=delay_t[44]*10;clear_y(35);};	
		if((X3)&&(!pre_x3)&&(!f_x3)) os_create_task(TASK_X3);

		pre_x3=X3;	
		if((X19)&&(!pre_x19)){t_36=delay_t[30]*10;clear_y(36);};
		pre_x19=X19;

		x20=X20;
		if((x20)&&(!pre_x20))t_disable_y2=delay_t[43]*10;
		if(t_disable_y2) t_disable_y2--;
		pre_x20=x20;
		
			//y23 ----------------
		x35=X35;
		y23=shadow_y23;
		if(y23&&!pre_y23)
		{
			//if(x35sx48)
			if(delay_t[62]&0x01)
			{
				if(x35)t_y48_on=delay_t[50]*10;
					else {set_y(48);x48_check=1;t_x48_check=ERROR_CHECK_TIME;shadow_y49=0;}
			}
		}
		pre_y23=y23;
		//y23--------end---
		
		if(X5)f_x5=1;
		if((X44)&&(!pre_x44)&&f_x5)
		{t_24=delay_t[31]*10;t_37=delay_t[10]*10;clear_y(37);f_x5=0;};
		pre_x44=X44;
		
		//x26 ----------------
		x26=X26;
		if(x26&&!pre_x26)
		{
			if(error==126) error=0;
			
		}
		pre_x26=x26;
		//x26 end
		
		//x27 ----------------
		x27=X27;
		if(x27&&!pre_x27)
		{
			if(error==127) error=0;
			
		}
		pre_x27=x27;
		//x27 end
		
			//X33--------------------------
		x33=X33;
		if((x33&&!pre_x33)&&(t2d5s==0))
		{
			t29=delay_t[28]*10;
			t2d5s=250;
		}
		if(t2d5s)t2d5s--;
		
		if(t29)t29--;
		if(t29==1){set_y(50);clear_y(39);t05s=50;}
		
		if(t05s)t05s--;
		if(t05s==1){set_y(51);t1s=100;		}
		
		if(t1s)t1s--;
		if(t1s==1){clear_y(50);t02s=30;		}
		
		if(t02s)t02s--;
		if(t02s==1){set_y(39);clear_y(51);}
		
		pre_x33=x33;
		
		//x34 ----------------
		//x34=X34;
		x34=X34;//use X4 stop key to save
		if((x34)&&pre_x34&&(!ppre_x34))
		{if((error==0||error>=100))error=34;stop();}
		
		if(x34&&pre_x34)ppre_x34=1;
		if(!x34&&!pre_x34)ppre_x34=0;
		pre_x34=x34;
		//end x34--------------
		
		//x1835------------		
		//x1835=X18;
		//if((x1835)&&(!pre_x1835))f_back_enable_x18=1;
				
		pre_x1835=x1835;
		//end x1835
		
			//X35--------------------------
		x35=X35;
		if(x35&&!pre_x35)
		{//------------------
			//----------------------
			if(!middle_store)
			{
				t9=delay_t[8]*10;
				f_back_enable_x18=1;
				t_y47off=600;
				set_y(47);
			}
			else
			{
				//x35sx48=1;
				delay_t[62]|=0x01;
				shadow_y49=0;
				//set_y(60);
				if(shadow_y23)t_y48_on=delay_t[50]*10;
			}
		}

		pre_x35=x35;
		//end X35----------------------
		
		//y41------------	
		if(t_y41on==1)
		{
			if(get_y(41))
			{
				clear_y(41);
				t_y41on=delay_t[47];
			}
			else if(count_y41)
			{
				set_y(41);
				t_y41on=delay_t[47];
				count_y41--;
			}
		}
		if(t_y41on)		t_y41on--;
		//end y41
		
		//x46 ----------------
		x46=X46;
		if(!x46&&!pre_x46&&ppre_x46)//up
		{
			t_y11_on=delay_t[53]*100;
			if((error==0||error>=100))error=46;
			stop();
			
		}
		
	
	//	else
	//	{
	//		if(error==46)	error=0;
	//		t_y11_on=0;
	//		clear_y(11);
	//	}
		
		if(x46&&pre_x46)ppre_x46=1;
		if(!x46&&!pre_x46)ppre_x46=0;

		pre_x46=x46;
		
		//end x46--------------
		if(t_y11_on)t_y11_on--;
		if(t_y11_on==1)set_y(11);
		
		
		//x48 ----------------
		//if(AFTER_X48)		x48=X48;
		//else x48=0;
		if(AFTER_X48)		
		{
			x48=X48;
			x49=X49;
			x50=X50;
			x51=X51;
			x52=X52;
			x53=X53;
			x54=X54;
			x55=X55;
//			x56=X56;
			x57=X57;
		}
		else 
		{
			x48=0;
			x49=0;
			x50=1;
			x51=1;
			x52=1;
			x53=0;
			x54=0;
			x55=0;
		//	x56=0;
			x57=0;
		}

		if(x48&&pre_x48&&!ppre_x48&&get_y(48))
		{
			//x35sx48=0;
			x48_check=0;
			delay_t[62]&=~0x01;
			//clear_y(60);
			shadow_y49=0;
			t_y48_off=delay_t[49]*10;
			
			if(middle_store)
			{
				t9=delay_t[8]*10;
				f_back_enable_x18=1;
				t_y47off=600;
				set_y(47);
				
			}
		}
		

		if(x48&&pre_x48)ppre_x48=1;
		if(!x48&&!pre_x48)ppre_x48=0;

		pre_x48=x48;

		//end x48--------------
		
			//x49 ----------------
		//if(AFTER_X48)		x49=X49;
		//else x49=0;
		if(x49&&!pre_x49)
		{
			
			x49_check=0;

		}
		pre_x49=x49;
		//end x49--------------
		
		if((!x50)&&(!pre_x50))
		{if((error==0||error>=100))error=50;stop();}
		//else if(error==50)error=0;
		if((!x51)&&(!pre_x51))
		{if((error==0||error>=100))error=51;stop();}
		//else if(error==51)error=0;
		if((!x52)&&(!pre_x52))
		{if((error==0||error>=100))error=52;stop();}
		//else if(error==52)error=0;
		if(x53&&pre_x53)
		{if((error==0||error>=100))error=53;stop();}
		//else if(error==53)error=0;
		if(x54&&pre_x54)
		{if((error==0||error>=100))error=54;stop();}
		//else if(error==54)error=0;
		
		pre_x50=x50;
		pre_x51=x51;
		pre_x52=x52;
		pre_x53=x53;
		pre_x54=x54;

		//x55 ----------------
		//x55=X55;	   
		//x57=X57;
		if(x55&&pre_x55&&!ppre_x55&&!x57&&(status_updown==57))//up
		{
			//if((status_updown==56)||(status_updown==57))
			//{
				t_y53off=0;
				clear_y(53);
				error=55;
				stop();
				t_y52off=delay_t[22]*100;
				
				
				status_updown=0;
			//}
		}
		else if(!x55&&pre_x55)//down
		{
			status_updown=55;
		}
		if(!x55&&!pre_x55)ppre_x55=0;
		if(x55&&pre_x55)ppre_x55=1;
		pre_x55=x55;
		//end x55--------------
		
		//x56 ----------------
		/*
		x56=X56;
		if(x56&&!pre_x56)//up
		{
		
		}
		else if(!x56&&pre_x56)//down
		{
			
		}
		pre_x56=x56;*/
		//end x56--------------
		
		//x55 ----------------
		//x57=X57;
		if(x57&&pre_x57&&!ppre_x57&&!x55&&(status_updown==55))//up
		{
			//if((status_updown==56)||(status_updown==55))
			//{
				t_y52off=0;
				clear_y(52);
				error=57;
				stop();
				t_y53off=delay_t[22]*100;
				
				status_updown=0;
				
					
			//}
		
		}
		else if(!x57&&pre_x57)//down
		{
			status_updown=57;
		}
		if(!x57&&!pre_x57)ppre_x57=0;
		if(x57&&pre_x57)ppre_x57=1;

		pre_x57=x57;
		//end x57--------------
		
		//X58--------------------------------------------------------
		if(middle_store)//have middle
		{
			if(shadow_y49)set_y(23);
			else clear_y(23);
		}
		else
		{
			if(shadow_y23)set_y(23);
			else clear_y(23);
		}
		//X58-----------output shadow_y23 or shadow_y49 to Y23 line---
		
		if(t_y53off)
		{	clear_y(52);	set_y(53);t_y53off--;}
		else {clear_y(53);}
		if(t_y52off)
		{	clear_y(53);	set_y(52);t_y52off--;}
		else {clear_y(52);}
		//end x55 x56 x57
		
		//if((!x35sx48)&&(X49))shadow_y49=1;
		if((!(delay_t[62]&0x01))&&(X49))shadow_y49=1;
		if(X49){if(error==149)error=0;}
		else if((error==0)&&(middle_store))  error=149;
			
		if(t_y48_on==1){set_y(48);x48_check=1;t_x48_check=ERROR_CHECK_TIME;shadow_y49=0;}
		if(t_y48_on)t_y48_on--;
		
		if(t_y48_off==1){clear_y(48);x49_check=1;t_x49_check=ERROR_CHECK_TIME;}
		if(t_y48_off)t_y48_off--;
				
		if(t_x48_check==1){if(x48_check){error=48;stop();}}
		if(t_x48_check)t_x48_check--;
		
		if(t_x49_check==1){if((x49_check)&&(middle_store)){error=49;stop();}}
		if(t_x49_check)t_x49_check--;
		
		//----x18---x4---x37--y5--y40----
		x18=X43;
		//x4=X2;
		x37=X37;
		
	
		if((get_y(26))&&(x18)&&(!pre_x18))
		{
			if(delay_t[7]==0)set_y(40);
			else			t_y40=delay_t[7];//set_y(40);
			set_y(5);
			if(delay_t[17]==0)delay_t[17]=1;
			t_x18=delay_t[17]*10;
		}
		//if((!x4)&&(pre_x4))
		if(t_x18==1)
		{
			t_y40=0;
			clear_y(40);
			if(delay_t[20]==0)clear_y(5);
			else	t_y5=delay_t[20];//clear_y(5);
		}
		
		if(t9)t9--;
		if(t_y40==1){set_y(40);}
		if(t_y5==1){clear_y(5);}
		
		if(t_y40)t_y40--;
		if(t_y5)t_y5--;
		if(t_x18)t_x18--;
		

		
		if(t_y42_off==1){clear_y(42);}
		if(t_y42_off)t_y42_off--;
		
		if(t_y43_off==1){clear_y(43);set_y(44);t_y44_off=delay_t[26]*10;}
		if(t_y43_off)t_y43_off--;
		
		if(t_y44_off==1){clear_y(44);}
		if(t_y44_off)t_y44_off--;
		
		//add by 20170429
		
		if(t_y47off)
		{
		
			t_y47off--;
	
		}
		else
		{
			clear_y(47);
		//	t_y47=0;
		} 
		//end add by 20170429
		
		//----------------------
		pre_x18=x18;
	//	pre_x4=x4;
		pre_x37=x37;
		//----------------
		
//--------------------------
		x45=X45;
		if(x45&&pre_x45&&!ppre_x45)
		{
			t_ystop=delay_t[9]*10;
		
		}
		if((!x45)&&(!pre_x45))
		{
			ppre_x45=0;
		}
		if(x45&&pre_x45)ppre_x45=1;
	
		if(t_ystop==1)
		{
			
			if((error==0)||(error>100))error=45;
			stop();
			xstop=0;
			clear_y(30);
			clear_y(31);
			clear_y(32);
			clear_y(35);
			if(!X58)clear_y(36);
			clear_y(37);
			clear_y(39);		
		}
		if(t_ystop)t_ystop--;
		

		pre_x45=x45;
		
//--------------------------
 	
		if(X42)f_x42=1;
		
	//	x2=X2;
	//	if(x2&&!pre_x2&&f_x2_enable){clear_y(20);t_y20=50;}
	//	if(t_y20)t_y20--;
	//	if((t_y20==1)&&f_x2_enable){set_y(20);}
	//	pre_x2=x2;

	
		}
	}
}

//---------------ok----------------------
void task_x3()	_task_ TASK_X3
{
	unsigned char i=0;
	f_x3=1;	
	os_wait2(K_TMO,delay_t[28]*10);
		t_35=delay_t[29]*10+1;
		clear_y(35);
		movein();
   /*
		while(1)
		{
			if(X20)
			{
				t_35=delay_t[43]*10+1;
				i=delay_t[43];
			}		
			if(i)
			{
				i--;
				os_wait2(K_TMO,10);
			}
			else break;
		}
	*/
		if(X20)
		{
			t_disable_y2=delay_t[43]*10;
			
		}
		else t_disable_y2=0;
		
		while(t_disable_y2){os_wait2(K_TMO,10);t_35=delay_t[29]*10;};				
		set_y(2);
			
	//	t_35=delay_t[29]*10;
	//	clear_y(35);
		
		os_wait2(K_TMO,delay_t[29]*10);
		clear_y(2);
		t_35=0;
		//set_y(35);
		
		f_x3=0;
		os_delete_task(TASK_X3);
		
}
void give_bag() _task_	GIVE_BAG 
{
	static bit f_e6=0;
	unsigned char i;
//	printf("task GIVE_GAG started!\n");
	while(1)
	{
		movein();
		moveout();
		if(Y63==0)
		{//--------------------	
			if(status==RUNING)
			{
				if(X9)
				{
					//1---------
					task_status[GIVE_BAG]=1;
					while(!X6){f_e6=1;os_wait2(K_TMO,10);if(error==0)error=106;}
					
					//2---------
					
					task_status[GIVE_BAG]=2;
					if(f_e6)
					{
						os_wait2(K_TMO,250);
						os_wait2(K_TMO,250);
				//		os_wait2(K_TMO,250);
				//		os_wait2(K_TMO,250);
						f_e6=0;
						if(error==106)error=0;
					};
					if(status==STOP)continue;
						
					//3---------
					task_status[GIVE_BAG]=3;
					if((!X7)&&(X10))
					{
						i=0;
						os_wait2(K_TMO,10);
						if(!X7)i++;
						os_wait2(K_TMO,10);
						if(!X7)i++;
						os_wait2(K_TMO,10);
						if(!X7)i++;
												
						if((i>=2)&&(X6))set_y(7);
						/*if(delay_t[0]>25)
						{
							os_wait2(K_TMO,250);
							os_wait2(K_TMO,(delay_t[0]-25)*10);
						}
						else	os_wait2(K_TMO,delay_t[0]*10);
						*/
						for(i=0;i<40;i++)
						{
							os_wait2(K_TMO,10);
						}
						clear_y(7);
						if(!X7){if(error==0)error=107;};
					}
					
				}
		
			}
		}
		os_wait2(K_TMO,2);
	}
}
//------------------------------------------
void get_bag()	_task_	GET_BAG
{
	static bit pre_status=STOP;		
	bit e7;
	unsigned char	count_t;
//	printf("task GET_GAG started!\n");
	
	while(1)
	{
		movein();
		moveout();
		if(Y63==0)
		{
			if(status==RUNING)
			{
				if(pre_status==RUNING) //normal runing.......
				{
					//1---------
					task_status[GET_BAG]=1;
				//	if(!X9){if((error==0)||(error>100))error=9;stop();}
					while(!X9){os_wait2(K_TMO,5);if(error==0)error=109;}
					if(error==109)error=0;
					while(!X10){os_wait2(K_TMO,5);if(error==0)error=110;}
					if(error==110)error=0;
				//	else if(!X10){if((error==0)||(error>100))error=10;stop();}
				//	else
					{
						while(!X7){os_wait2(K_TMO,5);e7=1;if(error==0)error=107;};
						
						if(error==107)error=0;
						if(e7){os_wait2(K_TMO,100);e7=0;}
						while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
						os_wait2(K_TMO,50);
						set_y(10);
						set_y(34);
						os_wait2(K_TMO,delay_t[1]*10);
						set_y(12);
						os_create_task(PRESS_12);
						clear_y(10);
						os_wait2(K_TMO,20);
						
						
						//3------------------
						task_status[GET_BAG]=3;
						count_t=0;
						while(!X11)
						{
							os_wait2(K_TMO,1);
							if(++count_t>=((delay_t[15]-delay_t[1])*10))
								{if((error==0)||(error>100))error=11;clear_y(34);stop();}
						}
						clear_y(34);
						os_wait2(K_TMO,1);
			
						count_t=0;
					/*	while(!X9)
						{
							os_wait2(K_TMO,1);
							if(++count_t>=((250)*10))
								{if((error==0)||(error>100))error=9;stop();}
						}
						if(!X9){if((error==0)||(error>100))error=9;stop();}
						else*/
						while(!X9){os_wait2(K_TMO,1);if(error==0)error=109;}
						if(error==109)error=0;
						{
											
							clear_y(12);
							t_3=delay_t[45]*10;
							os_delete_task(PRESS_12);
							set_y(13);
							os_create_task(PRESS_13);
							
							os_wait2(K_TMO,delay_t[2]*10);
													
							//waiting......................
							os_wait1(K_SIG);
						
							while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
							set_y(33);
							//4-------------
							task_status[GET_BAG]=4;
							count_t=0;
							while(!X10)
							{
								os_wait2(K_TMO,1);
								if(++count_t>=(delay_t[16]*10))
									{if((error==0)||(error>100))error=10;clear_y(33);stop();}
							}
						//	while(!X10){os_wait2(K_TMO,1);if(error==0)error=110;}
							if(error==110)error=0;
							clear_y(33);
							
							task_status[GET_BAG]=5;						
							count_t=0;
					
							while(X8)
							{
								os_wait2(K_TMO,2);
							//	if(++count_t>=(250*10))
							//		{error=8;stop();}
								while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
							
							}
							
							if((!X8)&&(!get_y(8)))
							{
								clear_y(13);
								os_delete_task(PRESS_13);
								os_wait2(K_TMO,2);
							
							set_y(14);
							os_wait2(K_TMO,delay_t[19]*10);
							clear_y(14);
							if(f_get_error==0){os_send_signal(ARRANGE_BAG);}
							else {os_send_signal(GET_BAG);}
							f_get_error=0;
			
							}
							
						}
					}
				}
					//-----------------
				else				 //first runing....
				{
					task_status[GET_BAG]=0;
					count_t=0;
					set_y(33);
					while(!X10)
					{
						os_wait2(K_TMO,1);
						if(++count_t>=(delay_t[16]*10))
							{if((error==0)||(error>100))error=10;clear_y(33);stop();}
					}
					//while(!X10){os_wait2(K_TMO,1);if(error==0)error=110;}
					if(error==110)error=0;
					clear_y(33);
				
					os_wait2(K_TMO,1);
				//	if(X10)		os_send_signal(ARRANGE_BAG);
					
				}
		
			}
			pre_status=status;
		}
		os_wait2(K_TMO,2);
	}
}
void arrange_bag()	_task_    ARRANGE_BAG
{
	unsigned char i;
	unsigned char count_t;
//	printf("task ARRANGE_GAG started!\n");
	
	task_status[ARRANGE_BAG]=0;
	
	set_y(15);
	moveout();
	os_wait2(K_TMO,250);
	os_wait2(K_TMO,250);
	clear_y(15);
	moveout();
	os_send_signal(GET_BAG);
	
	while(1)
	{
		movein();
		moveout();
		if(status==RUNING)
		{
			os_wait1(K_SIG);
			while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
			
			//1-------------
			task_status[ARRANGE_BAG]=1;

			os_wait2(K_TMO,delay_t[3]*10);
			if(delay_t[18]>0)
			{
				for(i=0;i<delay_t[18]-1;i++)
				{
					set_y(9);
					os_wait2(K_TMO,delay_t[4]*10);
					clear_y(9);
					os_wait2(K_TMO,delay_t[4]*10);
				}
				set_y(9);
				os_wait2(K_TMO,delay_t[4]*10);
				clear_y(9);
			}
			//2-------------
			task_status[ARRANGE_BAG]=2;			
			//wait for X14
			//while(!X14)
			//{
			//	os_wait2(K_TMO,5);
			//}
			count_t=0;
			if(!X8){set_y(8);}
			os_wait2(K_TMO,40);
			while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
			while(!X8)
			{
				set_y(8);
				os_wait2(K_TMO,1);
			//	if(++count_t>=100)	{break;}
			//	if(++count_t>=250*10)
			//			{error=8;clear_y(8);stop();break;}
				movein();
			}
			clear_y(8);
			moveout();
			
			//3-------------
			task_status[ARRANGE_BAG]=3;
			os_wait2(K_TMO,2);
			if(X8)	
			{
		//		os_send_signal(UP_BAG);
			
				
			}
			os_send_signal(GET_BAG);
			
		}
		os_wait2(K_TMO,2);
	}
}
void open_bag()	_task_	OPEN_BAG
{
	unsigned char count_t;
	while(1)
	{
		movein();
		moveout();
		if(status==RUNING)
		{
			
			//wait for X14,X8,X16,twice
		
			while(1)
			{
				os_wait2(K_TMO,2);
				movein();
				if((X14)&&(X8)&&(X16)&&(!f_open)&&(!t_8))
				{
					if((error==114)||(error==108)||(error==116))error=0;
					break;
				}
				if(error==0){	if(!X14)error=114;}
				if(error==0){if(!X8)error=108;}
				if(error==0){if(!X16)error=116;}
			}
			task_status[OPEN_BAG]=2;
			os_wait2(K_TMO,5);
			while(1)
			{
				os_wait2(K_TMO,2);
				movein();
				if((X14)&&(X8)&&(X16)&&(!f_open)&&(!t_8))
				{
					if((error==114)||(error==108)||(error==116))error=0;
					break;
				}
				if(error==0){	if(!X14)error=114;}
				if(error==0){if(!X8)error=108;}
				if(error==0){if(!X16)error=116;}
			}

			while(status==STOP){os_wait2(K_TMO,2);};
	
			//set y18 and wait for X17
			set_y(18);
			count_t=0;
			while(!X17)
			{
				os_wait2(K_TMO,1);
				if(++count_t>=250)
					{if((error==0)||(error>100))error=17;stop();}
			}
			task_status[OPEN_BAG]=3;
			//wait for X33 ,to open bag
			if(!X8){clear_y(18);if((error==0)||(error>100))error=8;stop();continue;}
			
			//if(f_first)f_first=0;
			//else
			//{
				/*
				while(1)
				{
					os_wait2(K_TMO,2);
					if(get_y(33)||(X10))break;
		
				}*/
			//}
		
			//open...
			os_wait2(K_TMO,delay_t[41]*10);
			set_y(16);
			os_create_task(PRESS_22);
			os_wait2(K_TMO,delay_t[6]*10);
			clear_y(18);
			
			f_open=1;

			//5-------------
			task_status[OPEN_BAG]=4;
			count_t=0;
						
			//wait for X16
			count_t=0;
			while(!X16)
			{
				os_wait2(K_TMO,2);
				clear_y(18);
				if(++count_t>250)	{if((error==0)||(error>100))error=16;	stop();	}
			}
			//	while(!X16){os_wait2(K_TMO,2);if(error==0)error=116;}
				if(error==116)error=0;
			//open finished
		//	os_wait2(K_TMO,delay_t[42]);
		/*	if((status==STOP)&&(error==22))
				{f_open=0;while(status==STOP)os_wait2(K_TMO,2);}
			else {f_open=1;}
			*/
			os_send_signal(UP_BAG);
			while(status==STOP)os_wait2(K_TMO,2);
			task_status[OPEN_BAG]=5;
		
		}
		os_wait2(K_TMO,10);
	}	
}
void up_bag()	_task_	UP_BAG
{
	unsigned char count_t;
//	unsigned char dt25; 
	bit shadow_x35;
	
	static unsigned char count_36=0;
//	printf("task UP_GAG started!\n");	
	//first time push_pack;
	os_send_signal(PUSH_PACK);
	//f_back_enable=1;
	while(1)
	{
		movein();
		moveout();
		if(status==RUNING)
		{
			task_status[UP_BAG]=0;
			os_wait1(K_SIG);
		//	if(!X46){if((error==0)||(error>100))error=46;stop();};
			while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
			
			//1-------------
			task_status[UP_BAG]=1;
	
		//	if(X8)
		//	{
		//		if(X14)
		//		{
					while(1)
					{
					
						task_status[UP_BAG]=2;
						//wait for 8
						while(!X8)	{		os_wait2(K_TMO,2);if(error==0)error=108;}		
						//wait for 14			
						//while(!X14)	{		os_wait2(K_TMO,2);if(error==0)error=114;}		
						while(!X14)
						{
							os_wait2(K_TMO,1);
							if(++count_t>=250)		{error=14;stop();}
						}
						while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
							
						//wait for 16			
						while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
						while(!X16)	{		os_wait2(K_TMO,2);if(error==0)error=116;}	
						//wait for 25			
						while(X25)	{		os_wait2(K_TMO,2);if(error==0)error=125;}		
						//wait for 32			
						while(X32)	{		os_wait2(K_TMO,2);if(error==0)error=132;}	
						//wait for 23			
						while(X23)	{		os_wait2(K_TMO,2);if(error==0)error=123;}	
						//wait for 24			
						while(X24)	{		os_wait2(K_TMO,2);if(error==0)error=124;}			
					
						//wait for 26				
						while(!x26)	{		os_wait2(K_TMO,1);if(error==0)error=126;}
						
						while(!f_open)	{		os_wait2(K_TMO,1);/*if(error==0)error=150;*/}
						if((error==108)||(error==114)||(error==116)||(error==125)||(error==132)||(error==123)||(error==124)||(error==126))error=0;	
						while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
						movein();
						//if((X8)&&(X14)&&(X16)&&(x26)&&(!X25)&&(!X32)&&(!X23)&&(!X24)&&get_y(16))
						if((X8)&&(X14)&&(X16)&&(x26)&&(!X25)&&(!X32)&&(!X23)&&(!X24)&&(f_open)&&(!X27)&&((get_y(26))||(X26)))
							break;
					}
					
				{
					task_status[UP_BAG]=3;
					set_y(17);
					moveout();
							//-------------------
					if((X36)&&((count_36<(delay_t[0]*10))||(delay_t[0]==0)))
					{
					
						s_delay_13=delay_t[33];
						s_delay_21=delay_t[46];
						count_36++;
						count_36&=0xff;
						clear_y(44);
						set_y(45);
					}
					else 
					{
						if(!X36)count_36=0;
						s_delay_13=delay_t[13];
						s_delay_21=delay_t[21];
						clear_y(43);
						set_y(44);
						clear_y(45);
					}

					//6-------------
					task_status[UP_BAG]=6;				

					count_t=0;
					while(!X15)
					{
						os_wait2(K_TMO,1);
						if(++count_t>=250)
							{error=15;clear_y(17);stop();break;}
					}
					//while(!X15){os_wait2(K_TMO,5);if(error==0)error=115;}
					f_open=0;
					
					if(X15)
					{
						if(X14){if((error==0)||(error>=100))error=14;stop();}
						os_delete_task(PRESS_22);	
						os_wait2(K_TMO,delay_t[37]*10);
						if(error!=22)
						{
							set_y(22);
							set_y(42);
				
						}
						if(X8)	os_send_signal(UP_BAG);
					}
					os_wait2(K_TMO,15);
					
					
					//7-------------
					task_status[UP_BAG]=7;
				/*	while(!((X23)&&(X24)))
					{
						os_wait2(K_TMO,2);
					}
					*/
					clear_y(16);
					
					set_y(20);
					
					os_wait2(K_TMO,delay_t[27]*10);
					clear_y(20);
					clear_y(17);
					t_8=30;
					set_y(19);
			//		os_wait2(K_TMO,delay_t[27]*10);

					//8-------------
					task_status[UP_BAG]=8;
					
					if(X25){set_y(21);
						{clear_y(22);clear_y(47);}
						clear_y(42);
						if((error==0)||(error>100))error=25;
						stop();
						os_wait2(K_TMO,delay_t[39]*10);
						clear_y(21);
						}
					else if(X32){set_y(21);
						{clear_y(22);clear_y(47);}
						clear_y(42);
						if((error==0)||(error>100))error=32;
						stop();
						os_wait2(K_TMO,delay_t[39]*10);
						clear_y(21);
						}
					else if(!X23){set_y(21);
						{clear_y(22);clear_y(47);}
						clear_y(42);
						if((error==0)||(error>100))error=23;
						stop();
						os_wait2(K_TMO,delay_t[39]*10);
						clear_y(21);
						}
					else if(!X24){set_y(21);
						{clear_y(22);clear_y(47);}
						clear_y(42);
						if((error==0)||(error>100))error=24;
						stop();
						os_wait2(K_TMO,delay_t[39]*10);
						clear_y(21);
						}
					//else if(!X34)
						/*
						else if((!X34)||(!X33))	//20150403
						{set_y(21);
						{clear_y(22);clear_y(47);}
						clear_y(42);
						if((error==0)||(error>100))error=34;
						stop();
						os_wait2(K_TMO,delay_t[39]*10);
						clear_y(21);
						}*/
						//else if((!X46)||(!X2))	//20160922		????
						/*
						else if(!X46)
						{set_y(21);
						{clear_y(22);clear_y(47);}
						clear_y(42);
						if((error==0)||(error>100))error=46;
						stop();
						os_wait2(K_TMO,delay_t[39]*10);
						clear_y(21);
						}*/
					else 
					{
						shadow_y23=1;
						os_wait2(K_TMO,delay_t[27]*10);
						set_y(20);
						f_x2_enable=1;
						x41=1;
						os_wait2(K_TMO,2);
									
						//wait for X35
						//9-------------
						task_status[UP_BAG]=9;
					
						count_t=0;
						
					//	while(!X18)		{	os_wait2(K_TMO,2);}
					//	f_back_enable_x18=1;
								
						if(middle_store)
						{
							while(!X48)		{	os_wait2(K_TMO,2);if(error==0)error=148;}
							if(error==148)error=0;
							shadow_x35=X48;
						}
						else
						{		
							while(!X35)		{	os_wait2(K_TMO,2);if(error==0)error=135;}
							if(error==135)error=0;
							shadow_x35=X35;
						}	
						
					
						if(shadow_x35)
						{
							//add 20170429
							
							
							
							task_status[UP_BAG]=10;
							shadow_y23=0;
					
							//edit 20141008
				
							os_wait2(K_TMO,delay_t[5]*10);
					//		while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
								if(++delay_t[61]==0)++delay_t[60];
							check_counter0();
							counter0++;
							f_counter0=1;
							if(AUTH)
								{
									if(control_count)control_count--;
									else 
									{
										error=97;
										stop();
										printf("**-");
										os_delete_task(PUSH_PACK);
										os_wait2(K_TMO,100);
										os_delete_task(SYS_CON);
										os_wait2(K_TMO,100);
										os_delete_task(UP_BAG);
									}
								}							
						
					
							//---------add by 20160327
							if(X27){/*set_y(41)*/;f_y41=0;}
							else f_y41=1;
							t_y42_off=delay_t[22]*10;//clear_y(42);
							count_t=0;
							count_y41=delay_t[54];
							if(count_y41) {set_y(41);t_y41on=delay_t[47];count_y41--;}
							do
							{
								f_x42=0;
								
								set_y(28);
								os_wait2(K_TMO,delay_t[24]*10);
								clear_y(28);
								os_wait2(K_TMO,delay_t[24]*10);
								if(++count_t>=delay_t[14])break;
							}
							while(f_x42);
							
							t_y41on=0;
							clear_y(41);
							
							f_back_enable=1;
							//--------end of -add by 20160327
							
						//os_wait2(K_TMO,delay_t[8]*10);
							while(t9){os_wait2(K_TMO,1);};//wait for t9 delay
					//		if(X42)os_wait2(K_TMO,delay_t[40]*10);
						
							task_status[UP_BAG]=11;
					//		while(X42)os_wait2(K_TMO,10);
							if(middle_store)
							{
								while(X48)os_wait2(K_TMO,10);//add 20191031
							}
							else
							{
								while(X35)os_wait2(K_TMO,10);//add 20141008
							}
							while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
							
							os_send_signal(PUSH_PACK);
							f_y25=1;
						}
											
					}
								
				}
				/*
				else 
				{
					if(!X8)error=8;
					if(!X14)error=14;
					if(!X16)error=56;
					if(!x26)error=26;
					if(X25)error=25;
					if(X24)error=24;
					if(X23)error=23;
					if(X32)error=32;
					if(!f_open)error=50;
					//if(!get_y(16))error=8;
					
					stop();
				}*/
	//		}
	//	}
		}
		
		os_wait2(K_TMO,2);
	}
}
void check_counter0()
{
	
}
void push_pack()	_task_	PUSH_PACK
{
	unsigned int count_t;
	unsigned char dt41;
	unsigned char i;
	unsigned char t_x36;
//	static unsigned int count_36=0;
//	printf("task PUSH_PACK started!\n");	

	//wait fo X27

	//0-------------
	task_status[PUSH_PACK]=0;
	count_t=0;
	if((X14)&&(!X15))	set_y(27);
	while(!X27)
	{
		if(error==0)error=127;
		os_wait2(K_TMO,35);
		if(++count_t>100)	{if((error==0)||(error>100))error=27;clear_y(27);stop();	}
	}
	//	while(!X27){os_wait2(K_TMO,20);if(error==0)error=127;}
	if(X27)
	{clear_y(27);x26=0;if(error==127)error=0;}
	x41=0;

	while(1)
	{
		movein();
		moveout();
		if(status==RUNING)
		{
			//1-------------
			task_status[PUSH_PACK]=1;
			os_wait1(K_SIG);
			while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
		/*	if(!X27){if((error==0)||(error>100))error=27;stop();}
			
			else*/ 
			//while(!X27){os_wait2(K_TMO,1);if(error==0)error=127;}
			//if(error==127)error=0;
			
			count_t=0;
	
			while(!X27)
			{
				if(error==0)error=127;
				os_wait2(K_TMO,2);
				if(++count_t>150)	{if((error==0)||(error>100))error=27;clear_y(27);stop();	}
			}
			if(error==127)error=0;
			
				x26=0;
				if(X36)	t_x36=delay_t[36];
				else		t_x36=delay_t[32];
				
				if(t_x36<s_delay_21)
				{
				
				
					os_wait2(K_TMO,t_x36);
					task_status[PUSH_PACK]=2;
					{clear_y(22);clear_y(47);}
					if(s_delay_13==delay_t[33])
					{
						set_y(43);
						t_y43_off=delay_t[25]*10;
					}
					
					moveout();
					
					if(f_y25)
					{
						
						set_y(20);
						os_wait2(K_TMO,s_delay_21-t_x36);
						set_y(25);
						moveout();
						
					}
				}
				else
					
				{
					if(f_y25)
					{
						
						set_y(20);
						os_wait2(K_TMO,s_delay_21);
						set_y(25);
						moveout();
						
					}
				
					os_wait2(K_TMO,t_x36-s_delay_21);
					task_status[PUSH_PACK]=2;
					{clear_y(22);clear_y(47);}
					if(s_delay_13==delay_t[33])
					{
						set_y(43);
						t_y43_off=delay_t[25]*10;
					}
					moveout();
				}
					
				os_wait2(K_TMO,s_delay_13*10);
				clear_y(20);
				f_x2_enable=0;
				moveout();
			
				//while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				//2-------------
				task_status[PUSH_PACK]=3;
				count_t=0;
				if(f_y25)
				{
					while(!((X28)&&(X29)))
					{
						os_wait2(K_TMO,2);
						if(error==0)error=128;
						if(++count_t>250)	{if((error==0)||(error>100))error=28;	stop();	}
					}
				}
	
				os_wait2(K_TMO,delay_t[23]*10);//add 20150817
	
				task_status[PUSH_PACK]=4;
				while(!X41)
				{
					error=91;
					stop();
					os_wait2(K_TMO,10);
				}
				task_status[PUSH_PACK]=5;
				while(!X47)
				{
					error=47;
					stop();
					os_wait2(K_TMO,10);
				}
				task_status[PUSH_PACK]=6;
				while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				while(X45)
				{
					error=47;
					stop();
					os_wait2(K_TMO,10);
					while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				}
				while(!X14)
				{
					error=14;
					stop();
					os_wait2(K_TMO,10);
					while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				}

				while(!X18)
				{
					error=18;
					stop();
					os_wait2(K_TMO,10);
					while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				}

				//while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				task_status[PUSH_PACK]=7;
				set_y(26);
				f_back_enable_x18=0;
				
				moveout();
							
				count_t=0;				
				
				while(!X43)
				{
					os_wait2(K_TMO,1);
					movein();
					if(++count_t>250*2)	{if((error==0)||(error>100))error=43;	stop();break;	}
				}
				
				while(X43)
				{
					os_wait2(K_TMO,1);
					movein();
					if(++count_t>250*10)	{if((error==0)||(error>100))error=43;	stop();	break;}
				}
				task_status[PUSH_PACK]=8;
				if(!f_y25){set_y(21);os_wait2(K_TMO,delay_t[39]*10);clear_y(21);}
				f_y25=0;
				os_wait2(K_TMO,delay_t[42]);
				
		//		if(X43)
				{
					x26=1;
					for(i=0;i<delay_t[38];i++)
					os_wait2(K_TMO,10);
					clear_y(25);
					os_wait2(K_TMO,3);
							
				}
			
				
				//wait for X26..
				
				count_t=0;
				while(!X26)
				{
					os_wait2(K_TMO,2);
					if(++count_t>150)	
					{
						if((error==0)||(error>100))error=26;		
						xstop=0;
						stop();	
						clear_y(26);
						clear_y(25);
						//clear_y(41);
						
						clear_y(30);
						clear_y(31);
						clear_y(32);
						clear_y(35);
						if(!X58)clear_y(36);
						clear_y(37);
						clear_y(39);
					
					}
					
					if((error==0)||(error>100)){error=126;}
					
				}
				if(error==126)error=0;
				task_status[PUSH_PACK]=9;
				if(X26)
				{
					clear_y(26);
					clear_y(25);
					//clear_y(41);
					
					if(xstop&&(!X37)&&(!X38))//edit by v158
					{
						xstop=0;
					
						clear_y(30);
						clear_y(31);
						clear_y(32);
						clear_y(35);
						if(!X58)clear_y(36);
						clear_y(37);
						clear_y(39);
	
						
						
					}


				}
			//	os_wait2(K_TMO,20);
				
				task_status[PUSH_PACK]=10;
				while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				
				//3-------------
				//f_back_enable=1;

				//wait for paipaipai finished..
				//if(X47)while(!f_back_enable)	{		os_wait2(K_TMO,2);}

				//wait for X30..				
				//while(!X30)	{		os_wait2(K_TMO,2);if(error==0)error=130;}
				while(!X30)
				{
					os_wait2(K_TMO,1);
					if(error==0)error=130;
					movein();
					if(++count_t>250*2)	{if((error==0)||(error>100))error=30;	stop();break;	}
				}
				if(error==130)error=0;
				
				task_status[PUSH_PACK]=11;				
				//wait for X31..
				//while(!X31)	{		os_wait2(K_TMO,2);if(error==0)error=131;}
	
				//wait for X14..
				
			
				while(!x41)				{		os_wait2(K_TMO,2);/*	if(error==0)error=141;	*/	}
				while(!X14)				{		os_wait2(K_TMO,2);	if(error==0)error=114;		}
				while(!f_back_enable_x18){os_wait2(K_TMO,2);if(error==0)error=118;}
			//	os_wait2(K_TMO,20);
				if((error==130)||(error==131)||(error==141)||(error==114))error=0;
				while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				dt41=0;
				//if(!X47)
					{dt41=delay_t[40];
					f_back_enable=1;
					}
				
			task_status[PUSH_PACK]=12;
			while(!X18)
				{
					error=18;
					stop();
					os_wait2(K_TMO,10);
					while(status==STOP){os_wait2(K_TMO,5);};//if system stop,wait ...
				}
			while(!((X14)&&(X30)&&(X31)&&(X26)&&(x41)&&(!X15)&&(f_back_enable_x18)&&(X18)))//edit 20190406
				{
					if(!X14)if((error==0)||(error>100))error=114;
					if(!X30)if((error==0)||(error>100))error=130;
					if(!X31)if((error==0)||(error>100))error=131;
					if(!X26)if((error==0)||(error>100))error=126;
					if(!x41)if((error==0)||(error>100))error=141;
					if(!X18)if((error==0)||(error>100))error=118;
					
					os_wait2(K_TMO,2);
					
				}
				if((error==114)||(error==130)||(error==131)||(error==126)||(error==141)||(error==118))error=0;
				task_status[PUSH_PACK]=13;	
				os_wait2(K_TMO,dt41*10);
				set_y(27);x26=0;f_back_enable=0;
				f_first=0;
				
				//wait for X27..
				//4-------------
				task_status[PUSH_PACK]=14;
				count_t=0;
				os_wait2(K_TMO,10);
				/*while(!X27)
				{
					os_wait2(K_TMO,2);if(error==0)error=127;
				}
				*/
				while(!X27)
				{
					if(error==0)error=127;
					os_wait2(K_TMO,2);
					if(++count_t>150)	{if((error==0)||(error>100))error=27;clear_y(27);stop();	}
				}
				if(error==127)error=0;
				
				if(X27)
				{
					if(error==127)error=0;
				//	f_back_enable_x18=0;
					clear_y(27);
					x41=0;
					x26=0;
					if(f_y41){/*set_y(41);*/	f_y41=0;};
				}
			
				
			
		}
		os_wait2(K_TMO,2);
	}

}
void sew_pack()	_task_	SEW_PACK
{
	static bit pr_x38=0,pr_x37=0,x45=0;
	static unsigned char p37[5]={0,0,0,0,0};
	static unsigned char pp37=0;
	unsigned char sum_37;
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
			x37=X37;
			x38=X38;
			
			p37[pp37]=X37;
			if(pp37++>=4)pp37=0;
			
			if((status==RUNING)||xstop)
			{
				//1-------------
				task_status[SEW_PACK]=1;
				if(!t_35)set_y(35);
				if(!t_36)set_y(36);
				if(!t_37)set_y(37);
				set_y(30);
				set_y(31);
				//set_y(39);
		
				if((x37)||(x38))
				{
					if(!f_first)
					{
						set_y(32);
						if(X45)
						{
							task_status[SEW_PACK]=2;
							x45=1;		
							if((error==0)||(error>100))error=45;
						
						}
					}
				}
				
				sum_37=p37[0]+p37[1]+p37[2]+p37[3]+p37[4];
				if(sum_37>3)x37=1;
				else x37=0;
				
				if((!x37)&&pr_x37&&(!x38))	
				{
				
					task_status[SEW_PACK]=3;
					if((error==0)||(error>100))error=38;
					stop();
				}
				os_wait2(K_TMO,1);
				
				if(((!x38)&&(pr_x38))||((!x37)&&pr_x37&&(!x38)))
				{
					task_status[SEW_PACK]=4;
					if((!x38)&&(!x37))
						{
							os_create_task (CUT_LINE);
							clear_y(32);
						}

					if((x45)||(xstop))
					{
						task_status[SEW_PACK]=5;
						//os_wait2(K_TMO,delay_t[34]*10);
						for(i=0;i<delay_t[34]*5;i++)
						{
							if((x37)||(x38))
							{
								if(!f_first)	set_y(32);
							}
							os_wait2(K_TMO,2);
						}
						if(x45){x45=0;if((error==0)||(error>100))error=45;stop();}

						xstop=0;
										
						clear_y(30);
						clear_y(31);
						clear_y(32);
						clear_y(35);
						if(!X58)clear_y(36);
						clear_y(37);
						clear_y(39);
						
	
					}
				}
				
			}
			else
			{
				//3-------------
			//	task_status[SEW_PACK]=6;	
				
			}
			pr_x38=x38;
			pr_x37=x37;
		}
		os_wait2(K_TMO,2);
	}
}

void comm()	_task_	COMM
{
	//int i=0;
	unsigned char step=0;
	unsigned char ch;
	printf("started!\n");	
	//printf("Ver");
	while(1)
	{
		//	printf("%c",(step++));
		//os_wait2(K_TMO,2);
		
		ch=(_getkey ());
		//SBUF=ch;
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
				control_count=ch*20;
				step=0;
				//printf("##");
				if(ch==0)
				{
					error=98;
					stop();
					printf("**.");
					os_delete_task(PUSH_PACK);
					os_wait2(K_TMO,100);
					os_delete_task(SYS_CON);
					os_wait2(K_TMO,100);
					os_delete_task(UP_BAG);
				//	os_delete_task(COMM);
		
				}
				else
				{
					if(f_counter0)
					{
						printf("**P(%lu)",counter0);
						f_counter0=0;
					}
					//else printf("**AU(%d)",control_count&0x0ff);
				}
			
			break;
			default:
				step=0;
			
			
		}
		
	
	}
}

void cut_line() _task_ CUT_LINE
{
	unsigned char i;
	task_status[CUT_LINE]=1;
	os_wait2(K_TMO,delay_t[11]*10);
	for(i=0;i<delay_t[35];i++)
	{
		set_y(29);
		os_wait2(K_TMO,delay_t[12]*10);
		task_status[CUT_LINE]=2;	
		clear_y(29);
		os_wait2(K_TMO,delay_t[12]*10);
		task_status[CUT_LINE]=3;
	}
	task_status[CUT_LINE]=4;
	os_delete_task(CUT_LINE);
}						
void press_12() _task_ PRESS_12
{
	os_wait2(K_TMO,50);
	while(1)
	{	
		movein();
		moveout();
		if(Y63==0)
		{			
			if(!X12)	
			{
				clear_y(12);
				t_3=delay_t[45]*10;
				if(status==RUNING)if((error==0)||(error>100))error=12;
				f_get_error=1;
				moveout();
				stop();
		//		task_status[PRESS_CHECK]=12;
				os_delete_task(PRESS_12);
				
			}
		}	

		os_wait2(K_TMO,5);
	}
}
void press_13() _task_ PRESS_13
{
	os_wait2(K_TMO,50);
	
	while(1)
	{	
		movein();
		moveout();
		if(Y63==0)
		{
			if(!X13)	
			{
				clear_y(13);
				if(status==RUNING)if((error==0)||(error>100))error=13;
			
				moveout();
				stop();
		//		task_status[PRESS_CHECK]=13;
				os_delete_task(PRESS_13);
				
			}
		}
		os_wait2(K_TMO,5);
	}
}
void press_22() _task_ PRESS_22
{
	os_wait2(K_TMO,50);
	while(1)
	{	
		movein();
		moveout();
		if(Y63==0)
		{
			if((!X22)||(!X2))	
			{
				clear_y(16);
				if(status==RUNING)if((error==0)||(error>100)){if(!X22)error=22;else error=2;}
				f_open=0;
				moveout();
				stop();
		//		task_status[PRESS_CHECK]=2;
				os_delete_task(PRESS_22);
			}
		}
		os_wait2(K_TMO,5);
	}
}

void modbus()	_task_	MODBUS
{
		unsigned char i;
	eMBErrorCode    eStatus;
	
	f_save=0;
  eStatus = eMBInit( MB_RTU, 0x09, 2, 19200, MB_PAR_NONE ); 
  eStatus = eMBEnable(  );

//--------------- Main Loop -------------------------------------------------
	while(1)
	{
		( void )eMBPoll(  );
		os_wait2(K_TMO,2);
		if(f_save)
		{
	//		Iap_Erase(0);
			Iap_Write((unsigned int)0x200,(counter0>>24)&0x0ff);
			Iap_Write((unsigned int)0x201,(counter0>>16)&0x0ff);
			Iap_Write((unsigned int)0x202,(counter0>>8)&0x0ff);
			Iap_Write((unsigned int)0x203,counter0&0x0ff);
			for(i=0;i<64;i++)
			{
				Iap_Write((unsigned int)i,delay_t[i]);
			}
				
			//set_y(61);//fot test only
			f_save=0;
		}
		
	}

}


void start()
{
		if(t_y53off){error=57;return;}
		if(t_y52off){error=55;return;}
		if(X0)
			{
			xstop=0;	
			set_y(1);
			set_y(38);
			clear_y(4);
			os_wait2(K_TMO,3);
			status=RUNING;
	//		task_status[PRESS_CHECK]=22;
			error=0;
			clear_y(38);
				set_y(39);
	//		clear_y(46);
			}
}
void stop()
{
			if(status==RUNING)
			{
			set_y(4);
			clear_y(1);
			moveout();
			status=STOP;
			if(!X37&&!X38&&!((task_status[PUSH_PACK]>=2)&&(task_status[PUSH_PACK]<=9)))
			
				{	
					clear_y(30);
					clear_y(31);
					clear_y(32);
					clear_y(35);
					if(!X58)clear_y(36);
					clear_y(37);
					clear_y(39);
				}
				else xstop=1;
		

			}
			if(error==0)error=197;
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
								else if(iRegIndex<4+64)
								{
									*pucRegBuffer++ =0;
									*pucRegBuffer++ = delay_t[(iRegIndex-4)];
							
								}
								else if(iRegIndex<4+64+15)
								{
									*pucRegBuffer++ = 0;
									*pucRegBuffer++ = task_status[iRegIndex-4-64];
							
								}
								else if(iRegIndex==4+64+15)
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
									else if(iRegIndex<4+64)
									{
										pucRegBuffer++ ;
										delay_t[iRegIndex-4]=*pucRegBuffer++ ;
									//	f_save=1;				
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
						if(iRegIndex==5)
						{
						*pucRegBuffer++ = (counter0>>24)&0x0ff;
            *pucRegBuffer++ = (counter0>>16)&0x0ff ;
						//*pucRegBuffer++ = (counter0>>8)&0x0ff;
            //*pucRegBuffer++ = counter0&0x0ff ;
            	
						}
						else if(iRegIndex==4)
						{
						//*pucRegBuffer++ = (counter0>>24)&0x0ff;
            //*pucRegBuffer++ = counter0>>16&0x0ff ;
						*pucRegBuffer++ = (counter0>>8)&0x0ff;
            *pucRegBuffer++ = counter0&0x0ff ;
            	
						}
						
						else if(iRegIndex<4)
							
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



union AD_recive_packed    
{    
signed long weight_L; 
unsigned char L_weight[4];   
}AD_recive_weight;

extern union Modbus_packed    
{    
	long weight_long; 
	unsigned char long_weight[4];   
}Modbus_weight;

extern int Range_Filter_reg,Strong_Filter_reg;
char ADcode_rcived;
extern unsigned long AD_Result0; 
unsigned char recive_AD_code_Counter;
extern unsigned short CRC16(unsigned char *puchMsg, unsigned short usDataLen);
extern void AD_F_process(void);
int AD_recive_CRC;
void recive_AD_code(unsigned char Data);
/**********************************************/
void recive_AD_code(unsigned char Data)		
{

//int px;

		switch (recive_AD_code_Counter)
		{
			case 0:
				if(Data==0xef)//slave
//				if(Data==0xeb)//slave
				{
					recive_AD_code_Counter++;
				}
			break;
			case 1:
			     AD_recive_weight.L_weight[3]=Data;
				 recive_AD_code_Counter++;
			break;
			case 2:
			     AD_recive_weight.L_weight[2]=Data;
				 recive_AD_code_Counter++;
			break;
			case 3:
			     AD_recive_weight.L_weight[1]=Data;
				 recive_AD_code_Counter++; //addr
			break;
			case 4:  
			     AD_recive_weight.L_weight[0]=Data;
				 recive_AD_code_Counter++;	
			break;
			case 5:  
			     AD_recive_CRC=Data;
				 AD_recive_CRC<<=8;
				 recive_AD_code_Counter++;	
			break;
			case 6:  
			     AD_recive_CRC|=Data;
				 recive_AD_code_Counter++;	
			break;

			case 7:  
//			         px=CRC16((unsigned char *)AD_recive_weight.L_weight,4);
//					if(AD_recive_CRC==px)
//					{
//						  AD_Result0=AD_recive_weight.weight_L;//COMM over ,get ADcode 等于原来AD转换结果
//							if(Strong_Filter_reg&&Range_Filter_reg)
//							 {
//							    AD_F_process();
//							 }
//					}	
				 recive_AD_code_Counter=0;	ADcode_rcived=1;
				 
			break;


	   //*************************************
           default:
		         recive_AD_code_Counter=0;
            break;
		}
}

void verify_filter_ADCODE(void)
{
int px;
if(!ADcode_rcived) return;
		 ADcode_rcived=0;
         px=CRC16((unsigned char *)AD_recive_weight.L_weight,4);
		if(AD_recive_CRC==px)
		{
			  AD_Result0=AD_recive_weight.weight_L;//COMM over ,get ADcode 等于原来AD转换结果
//			  if(AD_Result0==0)
//			  return;
			  if(Strong_Filter_reg==0)
			  {Strong_Filter_reg=1;}
				if(Strong_Filter_reg&&Range_Filter_reg)
				 {
				    AD_F_process();
				 }
		}	
}



//一
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
 //二，关于CS5532 SOD 的中断取数问题
 /*
 STM32 的EXTI中断是以中断响应程序void EXTIi_IRQHandler(void)对应中断线EXTI_Linei（i=0,1,2   15）	:
 EXTI中断源	0,1,2,3,4 独立的中断响应程序
 EXTI中断源	5,6 7,8,9, 对应中断响应程序void EXTI9_5_IRQHandler(void)｛｝	里面在分if (EXTI_GetITStatus(EXTI_Linej) != RESET)
{j=	5,6 7,8,9 }
EXTI中断源	10,11 12,13,14,15 对应中断响应程序void EXTI10_15_IRQHandler(void)｛｝	里面在分if (EXTI_GetITStatus(EXTI_Linej) != RESET)
{j=	0,11 12,13,14,15}

为了不影响SDO读数时引起中断(使中断效)，一个SDO对应两个GPIP:D8,B14(8,14在两个不同的中断组中)D8作为中断线，B14作为读数口。

必须在启动AD转换后，才能启动SDO的中断程序：
		WRITEBYTE(0xc0);//启动ADC
//		EXTI_NVIC_Configuration(); //启动EXTI 读AD
		EXTI_Configuration();
尤其是EXTI_Configuration();	一定要在 WRITEBYTE(0xc0);//启动ADC 后面，否则中断失败!!!!!
 */
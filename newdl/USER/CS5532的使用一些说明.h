//һ
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
	CLR_SCL();		//��һ�����ģ�������AD��̶�����ģ�����仯ʱ��Ҫ��λ��ų��µ����ݣ���SDO����ת������(�󲿷�ʱ��Ϊ�͵�ƽ)
	return(data);   //������һ�䣬AD����ģ�����仯����������SDO��Ϊ�ߵ�ƽ��SDO(�󲿷�ʱ��Ϊ�ߵ�ƽ)      
}
 //��������CS5532 SOD ���ж�ȡ������
 /*
 STM32 ��EXTI�ж������ж���Ӧ����void EXTIi_IRQHandler(void)��Ӧ�ж���EXTI_Linei��i=0,1,2   15��	:
 EXTI�ж�Դ	0,1,2,3,4 �������ж���Ӧ����
 EXTI�ж�Դ	5,6 7,8,9, ��Ӧ�ж���Ӧ����void EXTI9_5_IRQHandler(void)����	�����ڷ�if (EXTI_GetITStatus(EXTI_Linej) != RESET)
{j=	5,6 7,8,9 }
EXTI�ж�Դ	10,11 12,13,14,15 ��Ӧ�ж���Ӧ����void EXTI10_15_IRQHandler(void)����	�����ڷ�if (EXTI_GetITStatus(EXTI_Linej) != RESET)
{j=	0,11 12,13,14,15}

Ϊ�˲�Ӱ��SDO����ʱ�����ж�(ʹ�ж�Ч)��һ��SDO��Ӧ����GPIP:D8,B14(8,14��������ͬ���ж�����)D8��Ϊ�ж��ߣ�B14��Ϊ�����ڡ�

����������ADת���󣬲�������SDO���жϳ���
		WRITEBYTE(0xc0);//����ADC
//		EXTI_NVIC_Configuration(); //����EXTI ��AD
		EXTI_Configuration();
������EXTI_Configuration();	һ��Ҫ�� WRITEBYTE(0xc0);//����ADC ���棬�����ж�ʧ��!!!!!
 */
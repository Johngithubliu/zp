//v0.1
//#define SDA P4.5 
//#define SDA_IN() PM4|=0x20
//#define SDA_OUT() PM4&=0xdf
//#define SCL P4.4
//#define E_CS P4.3
//end v0.1

//v1.0
//#define SDA P4.3 
//#define SDA_IN() PM4|=0x08
//#define SDA_OUT() PM4&=0xf7
//#define SCL P4.4
//#define E_CS P4.5
//v1.0
/*
//fm3316
#define SDA_SET PDDATA|=0x10
#define SDA_CLR	PDDATA&=~0x10
#define SDA_DATA	(PDDIN&0x10?1:0)
#define SDA_IN() {PDPPEN&=~0x10;PDFCR1&=~0x10;_nop_();_nop_();_nop_();_nop_();}
#define SDA_OUT() {PDPPEN|=0x10;PDFCR1|=0x10;_nop_();_nop_();_nop_();_nop_();}
#define SCL_SET	PDDATA|=0x08
#define SCL_CLR	PDDATA&=~0x08
#define E_CS_SET PDDATA|=0x04
#define E_CS_CLR PDDATA&=~0x04
//fm3316
*/


//8031



#define SCL P24
#define E_CS P36
#define SDA P25

#define SDA_SET SDA=1
#define SDA_CLR	SDA=0
#define SDA_DATA	SDA
#define SDA_IN() {SDA=1;_nop_();}
#define SDA_OUT() _nop_()
#define SCL_SET	SCL=1
#define SCL_CLR	SCL=0
#define E_CS_SET E_CS=1
#define E_CS_CLR E_CS=0
//8031

 
extern unsigned char Iap_Read(unsigned int address);
extern void Iap_Write(unsigned int address,unsigned char dat);
//extern void Iap_Erase(unsigned char);
 
extern void read(unsigned char padd,unsigned char address,unsigned char *Pdata,unsigned char len); 
extern void write(unsigned char padd,unsigned char address,unsigned char *pdd,unsigned char len); 


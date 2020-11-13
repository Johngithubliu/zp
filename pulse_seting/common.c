#include "includes.h"

#if 01
void ByteToHexAscii(u8 chr, char *hi, char *lo)
{
	*hi = U8_HI(chr);
	*lo = U8_LO(chr);	
	
	if (*hi < 10) {
		*hi = *hi+'0';
	} else {
		*hi = *hi-10+'A';
	}

	if (*lo < 10) {
		*lo = *lo+'0';
	} else {
		*lo = *lo-10+'A';
	}	
}
#endif

#if 01
void ByteToDecAscii(u8 chr, char *hi, char *mi, char *lo)
{
	*hi = (chr/100)%10;
	*mi = (chr/10)%10;
	*lo = (chr)%10;
	

	if (*hi < 10) {
		*hi = *hi+'0';
	} else {
		*hi = *hi-10+'A';
	}


	if (*mi < 10) {
		*mi = *mi+'0';
	} else {
		*mi = *mi-10+'A';
	}
	

	if (*lo < 10) {
		*lo = *lo+'0';
	} else {
		*lo = *lo-10+'A';
	}	
}
#endif

#if 01
u8 HexAsciiToByte(u8 asc)
{
	if ('0'<=asc && asc<='9') {
		return asc-'0';
	} else if ('A'<=asc && asc<='F') {
		return asc-'A'+10;
	} else if ('a'<=asc && asc<='f') {
		return asc-'a'+10;
	} else {
	 	return 0;
	}
}
#endif

/*
线性插值函数
*/
#if 00
s32 LinearInterpolation(s32 x, s32 x0, s32 y0, s32 x1, s32 y1)
{
    return (y0)+(y1-y0)*(x-x0)/(x1-x0);
}
#endif

/*
抛物线插值函数
*/
#if 00
s32 ParabolaInterpolation(s32 x, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2)
{
	return LinearInterpolation(x,
			x1,
			LinearInterpolation(x, x0, y0, x1, y1),
			x2,
			LinearInterpolation(x, x0, y0, x2, y2));
}
#endif

#if 00
void ScientificNotationToFloat(fp64 *pFp, s32 mantissa, s32 exponent, u8 num)
{
	*pFp = mantissa;
	exponent -= num;
	if (exponent > 0) {
		for (exponent =  exponent; exponent > 0; exponent--) {
			*pFp *= 10;
		}
	} else {
		for (exponent = -exponent; exponent > 0; exponent--) {
			*pFp *= 0.1;
		}
	}
}
#endif

#if 00
void FloatToScientificNotation(fp64 fp, s32 *pMantissa, s32 *pExponent, u8 num_of_xiaosu)
{
	u32 low;
	u32 high;

	u8 sign;
	u8 i;



	//
	low = 1;
	for (i = num_of_xiaosu; i > 0; i--) {
		low *= 10;
	}
	
	high = low * 10;


	//
	if (fp == 0) {
		*pMantissa = 0;
		*pExponent = 0;
		
		return;
	} else if (fp < 0) {
		fp = -fp;
		sign = 1;
	} else {
		sign = 0;
	}	


	//
	if (fp >= high) {
		for (i = 0; fp >= high; i++) {
			fp *= 0.1;
		}
		
		*pExponent = num_of_xiaosu + i;
	} else if (fp < low) {
		for (i = 0; fp < low; i++) {
			fp *= 10;
		}
		
		*pExponent = num_of_xiaosu - i;
	} else {
		*pExponent = num_of_xiaosu;	
	}
	
	
	//
	if (sign == 0) {
		*pMantissa = fp;
	} else {
		*pMantissa = -fp;
	}
}
#endif
